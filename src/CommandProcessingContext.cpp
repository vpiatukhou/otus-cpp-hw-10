#include "CommandProcessingContext.h"
#include "ConsoleCommandWriter.h"
#include "DynamicCommandProcessor.h"
#include "FileCommandWriter.h"

namespace Homework {

CommandProcessingContext::~CommandProcessingContext() {
    stop();
}

void CommandProcessingContext::start(std::size_t blockSize) {
    commandWriters = { std::make_shared<ConsoleCommandWriter>(), std::make_shared<FileCommandWriter>() };
    staticCommandProcessor = std::make_unique<StaticCommandProcessor>(commandWriters, blockSize);
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
