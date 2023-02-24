#pragma once

#include <functional>
#include <string>

namespace Homework {

    /**
     * Parses the given input string.
     */
    class RequestParser {
    public:
        /**
         * Extracts commands from the given raw string
         * 
         * @param rawString - a request as a raw string. It may contain:
         *
         *                     1. A single command, ended with '\n'. E.g.:
         * 
         *                     CMD\n
         * 
         *                     2. Multiple commands. E.g.:
         * 
         *                     CMD1\nCMD2\nCMD3\n
         * 
         *                     3. A part of a command
         * 
         *                     CM
         * 
         *                     In the last case the given string will be added to the buffer and will be parsed after remained parts have come.
         */
        void parse(const std::string& rawString);

        /**
         * A callback function which is called after a command is extracted from the input string.
         * 
         * @param command - the extracted command
         */
        std::function<void(const std::string& command)> onCommandReceived;

    private:
        std::string buffer;
    };
}