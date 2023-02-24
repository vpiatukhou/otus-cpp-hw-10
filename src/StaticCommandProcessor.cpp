
#include "StaticCommandProcessor.h"

#include <utility>

namespace Homework {

StaticCommandProcessor::StaticCommandProcessor(std::vector<std::shared_ptr<FlushCommandListener>>& flushListeners_,  std::size_t blockSize_) 
    : flushListeners(flushListeners_), blockSize(blockSize_) {

    if (blockSize_ == 0) {
        throw std::invalid_argument("Block size must be greater than 0.");
    }
    commandBlock.reserve(blockSize_);
}

void StaticCommandProcessor::process(const std::string& command) {
    CommandBlock temp;
    {
        //This lock must be released before onFlush() is called because onFlush() acquires another lock.
        //We don't want to acquire more than one lock in order to avoid a deadlock.
        std::unique_lock<std::mutex> lock(commandBlockMutex);
        commandBlock.push_back(command);
        if (commandBlock.size() == blockSize) {
            temp = std::move(commandBlock);
            commandBlock.clear();
        }
    }
    flush(temp);
}

void StaticCommandProcessor::flush() {
    CommandBlock temp;
    {
        //This lock must be released before onFlush() is called because onFlush() acquires another lock.
        //We don't want to acquire more than one lock in order to avoid a deadlock.
        std::lock_guard<std::mutex> lock(commandBlockMutex);
        temp = std::move(commandBlock);
    }
    flush(temp);
}

void StaticCommandProcessor::flush(const CommandBlock& commands) {
    if (!commands.empty()) {
        for (auto& listener : flushListeners) {
            listener->onFlush(commands);
        }
    }
}

}
