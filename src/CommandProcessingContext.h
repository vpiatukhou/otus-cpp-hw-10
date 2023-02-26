#pragma once

#include "CommandProcessor.h"
#include "FlushCommandListener.h"
#include "StaticCommandProcessor.h"

#include <memory>
#include <vector>

namespace Homework {

    /**
     * Produces instances of CommandProcessor.
     * 
     * Holds objects which must be shared between all clients.
     */
    class CommandProcessingContext {
    public:
        CommandProcessingContext() = default;
        CommandProcessingContext(const CommandProcessingContext&) = delete;
        CommandProcessingContext(CommandProcessingContext&&) = delete;
        ~CommandProcessingContext();

        CommandProcessingContext& operator=(const CommandProcessingContext&) = delete;
        CommandProcessingContext& operator=(CommandProcessingContext&&) = delete;

        void start(std::size_t blockSize);

        /**
         * Processes remained commands and stops writer threads.
         * 
         * ATTENTION: This method blocks the caller until the commands are processed.
         */
        void stop();

        /**
         * Creates a new instance of CommandProcessor.
         */
        std::unique_ptr<CommandProcessor> createCommandProcessor();
    private:
        std::vector<std::shared_ptr<FlushCommandListener>> commandWriters;
        std::unique_ptr<StaticCommandProcessor> staticCommandProcessor;
    };
}
