#include "AsyncCommandWriter.h"

#include <thread>

namespace Homework {

    AsyncCommandWriter::AsyncCommandWriter(NumberOfThreads numberOfThreads_) : numberOfThreads(numberOfThreads_) {
        auto worker = [this](NumberOfThreads threadNumber) {
            auto predicate = [this] { return !commandBlocks.empty() || !isContinueProcessing; };

            while (isContinueProcessing || !isCommandQueueEmpty()) {
                CommandBlock block;
                {
                    std::unique_lock<std::mutex> lock(workerMutex);
                    continueProcessing.wait(lock, predicate);

                    if (!commandBlocks.empty()) {
                        block = std::move(commandBlocks.front());
                        commandBlocks.pop();
                    }
                }

                if (!block.empty()) {
                    write(block, threadNumber);
                }
            }
            ++numberOfFinishedThreads;
            blockProcessed.notify_all();
        };

        for (NumberOfThreads i = 0; i < numberOfThreads; ++i) {
            std::thread newThread(worker, i);
            newThread.detach();
        }
    }

    void AsyncCommandWriter::stop() {
        isContinueProcessing = false;
        continueProcessing.notify_all();

        //do not destroy object until all remained commands are processed
        std::unique_lock<std::mutex> lock(workerMutex);
        blockProcessed.wait(lock, [this] { return numberOfFinishedThreads == numberOfThreads; });
    }

    void AsyncCommandWriter::onFlush(const CommandBlock& commands) {
        {
            //this lock should be released before calling notify_one()
            //Please see https://en.cppreference.com/w/cpp/thread/condition_variable/notify_one for more details
            std::lock_guard<std::mutex> lock(workerMutex);
            commandBlocks.push(commands);
        }
        continueProcessing.notify_one();
    }

    bool AsyncCommandWriter::isCommandQueueEmpty() {
        std::lock_guard<std::mutex> lock(workerMutex);
        return commandBlocks.empty();
    }

}
