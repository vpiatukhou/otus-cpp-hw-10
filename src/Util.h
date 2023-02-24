#pragma once

#include "CommandBlock.h"

#include <ostream>

namespace Homework {

    /**
     * Writes the commands to the output stream.
     *
     * E.g. source contains [command 1, command 2, command 3].
     *
     * The following string will be written to the target:
     *
     * bulk: command 1, command 2, command 3
     *
     * NOTE: there is no newline character at the end of the string.
     */
    void writeCommandsToStream(const CommandBlock& source, std::ostream& target);

}
