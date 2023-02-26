#pragma once

#include "FlushCommandListener.h"

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <string>

namespace Homework {

    /**
     * The base class for an asynchronous command writer.
     *
     * Encapsulates thread management.
     */
    class AsyncCommandWriter : public FlushCommandListener {
    public:
        /**
         * Starts the threads.
         */
        AsyncCommandWriter();
        AsyncCommandWriter(const AsyncCommandWriter&) = delete;
        AsyncCommandWriter(AsyncCommandWriter&&) = delete;
        ~AsyncCommandWriter() = default;

        AsyncCommandWriter& operator=(const AsyncCommandWriter&) = delete;
        AsyncCommandWriter& operator=(AsyncCommandWriter&&) = delete;

        /**
         * Waits until remaining command blocks are written and stops all threads.
         *
         * ATTENTION: it is not guaranteed that the threads will be stopped immidiately.
         *
         * They may be stopped in some time since the method returns control.
         *
         * It is just guaranteed, that the mutex will be released and all commands will be processed.
         */
        void stop();

        /**
         * Puts the given block of commands to the queue.
         */
        void onFlush(const CommandBlock& commands) override;

    protected:
        using NumberOfThreads = unsigned int; //unsigned int has been chosen because it is returned by std::thread::hardware_concurrency()

        /**
         * The constructor.
         *
         * @param numberOfThreads_ - a number of threads which will be created.
         */
        AsyncCommandWriter(NumberOfThreads numberOfThreads_);

        /**
         * Writes the given block of commands.
         *
         * @param commands      - the block of commands
         * @param threadNumber  - a number to identify the thread
         */
        virtual void write(const CommandBlock& commands, NumberOfThreads threadNumber) = 0;

    private:
        bool isCommandQueueEmpty();

        NumberOfThreads numberOfThreads;
        std::queue<CommandBlock> commandBlocks;
        std::mutex workerMutex;

        std::condition_variable continueProcessing;
        std::condition_variable blockProcessed;

        std::atomic<NumberOfThreads> numberOfFinishedThreads{0};
        std::atomic_bool isContinueProcessing{true};
    };

}
