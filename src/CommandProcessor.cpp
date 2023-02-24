#include "CommandProcessor.h"

namespace Homework {

    CommandProcessor::CommandProcessor(StaticCommandProcessor& staticCommandProcessor_, std::vector<std::shared_ptr<FlushCommandListener>>& flushListeners_)
        : staticCommandProcessor(staticCommandProcessor_), dynamicCommandProcessor(flushListeners_) {
    }

    void CommandProcessor::process(const std::string& command) {
        if (!dynamicCommandProcessor.process(command)) {
            staticCommandProcessor.process(command);
        }
    }

}
