#pragma once

#include "DynamicCommandProcessor.h"
#include "StaticCommandProcessor.h"

namespace Homework {

    /**
     * Accumulates incoming commands into blocks and triggers FlushCommandListener.
     */
    class CommandProcessor {
    public:
        CommandProcessor(StaticCommandProcessor& staticCommandProcessor_, std::vector<std::shared_ptr<FlushCommandListener>>& flushListeners_);
        CommandProcessor(const CommandProcessor&) = delete;
        CommandProcessor(CommandProcessor&&) = delete;
        ~CommandProcessor() = default;

        CommandProcessor& operator=(const CommandProcessor&) = delete;
        CommandProcessor& operator=(CommandProcessor&&) = delete;

        void process(const std::string& command);

    private:
        StaticCommandProcessor& staticCommandProcessor;
        DynamicCommandProcessor dynamicCommandProcessor;
    };

}
