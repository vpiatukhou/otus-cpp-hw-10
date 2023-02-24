#pragma once

#include "FlushCommandListener.h"

#include <cstddef>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

namespace Homework {

    /**
     * Accumulates static commands and triggers FlushCommandListener when the command block is ready.
     * 
     * This class is thread-safe. It is assumed that all clients share the same instance.
     */
    class StaticCommandProcessor {

    public:
        StaticCommandProcessor(std::vector<std::shared_ptr<FlushCommandListener>>& flushListeners_, std::size_t blockSize_);
        StaticCommandProcessor(const StaticCommandProcessor&) = delete;
        StaticCommandProcessor(StaticCommandProcessor&&) = delete;
        ~StaticCommandProcessor() = default;

        StaticCommandProcessor& operator=(const StaticCommandProcessor&) = delete;
        StaticCommandProcessor& operator=(StaticCommandProcessor&&) = delete;

        void process(const std::string& command);
        void flush();
    private:
        std::size_t blockSize;
        CommandBlock commandBlock;
        std::mutex commandBlockMutex;

        std::vector<std::shared_ptr<FlushCommandListener>> flushListeners;

        void flush(const CommandBlock& commands);
    };
}