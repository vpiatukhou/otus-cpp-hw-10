#include "Util.h"

const std::string COMMAND_BLOCK_PREFIX = "bulk: ";
const std::string COMMAND_DELIMITER = ", ";

void Homework::writeCommandsToStream(const CommandBlock& source, std::ostream& target) {
    target << COMMAND_BLOCK_PREFIX << source[0];
    for (std::size_t i = 1; i < source.size(); ++i) {
        target << COMMAND_DELIMITER << source[i];
    }
}