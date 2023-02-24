#include "CommandProcessingContext.h"
#include "ConsoleCommandWriter.h"
#include "DynamicCommandProcessor.h"
#include "FileCommandWriter.h"

namespace Homework {

void CommandProcessingContext::init(std::size_t blockSize) {
    commandWriters = { std::make_shared<ConsoleCommandWriter>(), std::make_shared<FileCommandWriter>() };
    
    staticCommandProcessor = std::make_unique<StaticCommandProcessor>(commandWriters, blockSize);

    for (auto& writerPtr : commandWriters) {
        static_cast<AsyncCommandWriter*>(writerPtr.get())->start();
    }
}

void CommandProcessingContext::stop() {
    staticCommandProcessor->flush();

    for (auto& writerPtr : commandWriters) {
        static_cast<AsyncCommandWriter*>(writerPtr.get())->stop();
    }
}

std::unique_ptr<CommandProcessor> CommandProcessingContext::createCommandProcessor() {
    return std::make_unique<CommandProcessor>(*staticCommandProcessor, commandWriters);
}

}
