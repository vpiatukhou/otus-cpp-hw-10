
#include "DynamicCommandProcessor.h"

namespace Homework {

namespace {
    const std::string BEGIN_DYNAMIC_BLOCK_COMMAND = "{";
    const std::string END_DYNAMIC_BLOCK_COMMAND = "}";
}

DynamicCommandProcessor::DynamicCommandProcessor(std::vector<std::shared_ptr<FlushCommandListener>>& flushListeners_)
    : flushListeners(flushListeners_) {
}

bool DynamicCommandProcessor::process(const std::string& command) {
    bool processed = false;
    if (command == BEGIN_DYNAMIC_BLOCK_COMMAND) {
        ++openedDynamicBlockCounter;
        processed = true;
    } else if (command == END_DYNAMIC_BLOCK_COMMAND) {
        --openedDynamicBlockCounter;
        if (openedDynamicBlockCounter == 0 && !commandBlock.empty()) {
            flush();
        }
        processed = true;
    } else if (openedDynamicBlockCounter != 0) {
        commandBlock.push_back(command);
        processed = true;
    }
    return processed;
}

void DynamicCommandProcessor::flush() {
    for (auto& listener : flushListeners) {
        listener->onFlush(commandBlock);
    }
    commandBlock.clear();
}

}
