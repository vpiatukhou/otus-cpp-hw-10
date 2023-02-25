#include "Server.h"
#include "ShutdownHandler.h"

#include <boost/asio.hpp>

#include <cctype>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <limits>
#include <stdexcept>

namespace Homework {

    namespace {
        const int UNEXPECTED_ERROR = -1;
        const int VALIDATION_ERROR = -2;
        const std::uint8_t NUMBER_OF_ARGUMENTS = 3;
        const std::uint8_t PORT_ARG_INDEX = 1;
        const std::uint8_t BLOCK_SIZE_ARG_INDEX = 2;

        const Port MAX_PORT = 65535;

        const std::string PORT_AND_BLOCK_SIZE_NOT_POSITIVE_MSG = "PORT and BLOCK_SIZE must be positive numbers.";
        const std::string INVALID_PORT_MSG = "A port must be in the range [0-" + std::to_string(MAX_PORT) + "].";
        const std::string INVALID_NUMBER_OF_ARGUMETNS = "'bulk_server' requires 2 arguments.\n\nUsage:\tbulk_server PORT BLOCK_SIZE\n";
    }

    long stringToLong(const std::string& input) {
        for (auto character : input) {
            if (!std::isdigit(character)) {
                throw std::invalid_argument(PORT_AND_BLOCK_SIZE_NOT_POSITIVE_MSG);
            }
        }
        return std::stoul(input);
    }

    Port stringToPort(const std::string& portAsStr) {
        long number = stringToLong(portAsStr);
        if (number > MAX_PORT) {
            throw std::invalid_argument(INVALID_PORT_MSG);
        }
        return static_cast<Port>(number);
    }

    void readProgramArguments(int argc, char* argv[], Port& outputPort, std::size_t& outputBlockSize) {
        if (argc != NUMBER_OF_ARGUMENTS) {
            throw std::invalid_argument(INVALID_NUMBER_OF_ARGUMETNS);
        }
        outputPort = stringToPort(argv[PORT_ARG_INDEX]);
        outputBlockSize = static_cast<std::size_t>(stringToLong(argv[BLOCK_SIZE_ARG_INDEX]));
    }
}

int main(int argc, char* argv[]) {
    using namespace Homework;
    using boost::asio::ip::tcp;

    Port port;
    std::size_t blockSize;

    try {
        readProgramArguments(argc, argv, port, blockSize);
    } catch (std::invalid_argument& e) {
        std::cerr << e.what() << std::endl;
        return VALIDATION_ERROR;
    }

    CommandProcessingContext commandContext;
    commandContext.init(blockSize);

    startShutdownHandler(commandContext);

    try {
        boost::asio::io_context ioContext;

        Server server(ioContext, port, commandContext);
        server.start();

        std::cout << "The server is listening on the port " << port << '.' << std::endl;

        ioContext.run();
    } catch (const std::exception& e) {
        std::cerr << "Unexpected error occurred: " << e.what() << std::endl;
        return UNEXPECTED_ERROR;
    }

    return 0;
}
