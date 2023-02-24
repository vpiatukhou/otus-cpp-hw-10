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
        ~CommandProcessingContext() = default;

        CommandProcessingContext& operator=(const CommandProcessingContext&) = delete;
        CommandProcessingContext& operator=(CommandProcessingContext&&) = delete;

        /**
         * Prepares the context to work. Starts command writer threads.
         * 
         * This method must be called before any interaction with the context.
         */
        void init(std::size_t blockSize);

        /**
         * Processes remained commands. Stops command writer threads.
         * 
         * ATTENTION: This method blocks the caller thread until the remained commands are processed.
         * 
         * This method must be called before the instance of CommandProcessingContext is destroyed.
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
