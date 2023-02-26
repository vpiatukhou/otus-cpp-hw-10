#include "RequestParser.h"

#include <sstream>

namespace Homework {

    namespace {
        const char COMMAND_DELIMITER = '\n';
    }

    void RequestParser::parse(const std::string& rawInput) {
        //concatenate the buffer and the input string because 
        //the buffer can contain the beginning of a command and the input string can contain the end
        buffer += rawInput;

        std::size_t lastDelimiterIndex = buffer.find_last_of(COMMAND_DELIMITER);
        if (lastDelimiterIndex != std::string::npos && lastDelimiterIndex > 0) { //if there is \n in the given string => the buffer contains the complete command
            std::istringstream commandSequence;
            commandSequence.str(buffer);

            std::string command;
            while (std::getline(commandSequence, command, COMMAND_DELIMITER)) { //there may be more than one command in the rawInput
                onCommandReceived(command);
            }

            //rawInput is parsed only until the last '\n'.
            //All characters after '\n' are added to the buffer and may be parsed on the next call of parse()
            buffer = lastDelimiterIndex + 1 < rawInput.size() ? rawInput.substr(lastDelimiterIndex + 1) : "";
        }
    }
}