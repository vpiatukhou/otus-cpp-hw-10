#pragma once

#include "FlushCommandListener.h"

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

namespace Homework {

    /**
     * Accumulates dynamic commands and triggers FlushCommandListener when the command block is ready.
     * 
     * This class is NOT thread-safe.
     */
    class DynamicCommandProcessor {

    public:
        DynamicCommandProcessor(std::vector<std::shared_ptr<FlushCommandListener>>& flushListeners_);
        DynamicCommandProcessor(const DynamicCommandProcessor&) = delete;
        DynamicCommandProcessor(DynamicCommandProcessor&&) = delete;
        ~DynamicCommandProcessor() = default;

        DynamicCommandProcessor& operator=(const DynamicCommandProcessor&) = delete;
        DynamicCommandProcessor& operator=(DynamicCommandProcessor&&) = delete;

        /**
         * If the given command is '{' or '}', opens or closes a dynamic block respectively.
         * If the given command is NOT '{' or '}' AND a dynamic block is opened, adds the command to the block.
         * Otherwise - does nothing.
         * 
         * @param command a command to be processed
         * @return TRUE if a dynamic block has been opened or closed OR the command has been added to the block
         *         FALSE if the input is not a 'dynamic' command
         */
        bool process(const std::string& command);
    private:
        CommandBlock commandBlock;
        std::size_t openedDynamicBlockCounter = 0;

        std::vector<std::shared_ptr<FlushCommandListener>> flushListeners;

        void flush();
    };
}