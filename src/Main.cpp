#include "Server.h"
#include "ErrorCodes.h"

#include <boost/asio.hpp>

#include <cctype>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <limits>
#include <memory>
#include <stdexcept>

namespace Homework {

    namespace {
        const std::uint8_t NUMBER_OF_ARGUMENTS = 3;
        const std::uint8_t PORT_ARG_INDEX = 1;
        const std::uint8_t BLOCK_SIZE_ARG_INDEX = 2;

        const Port MAX_PORT = 65535;
    }

    long stringToLong(const std::string& input) {
        for (auto character : input) {
            if (!std::isdigit(character)) {
                throw std::invalid_argument("PORT and BLOCK_SIZE must be positive numbers.");
            }
        }
        return std::stoul(input);
    }

    Port stringToPort(const std::string& portAsStr) {
        long number = stringToLong(portAsStr);
        if (number > MAX_PORT) {
            throw std::invalid_argument("The port must be in the range [0-" + std::to_string(MAX_PORT) + "].");
        }
        return static_cast<Port>(number);
    }

    void readProgramArguments(int argc, char* argv[], Port& outputPort, std::size_t& outputBlockSize) {
        if (argc != NUMBER_OF_ARGUMENTS) {
            throw std::invalid_argument("'bulk_server' requires 2 arguments.\n\nUsage:\tbulk_server PORT BLOCK_SIZE\n");
        }
        outputPort = stringToPort(argv[PORT_ARG_INDEX]);
        outputBlockSize = static_cast<std::size_t>(stringToLong(argv[BLOCK_SIZE_ARG_INDEX]));
    }
}

int main(int argc, char* argv[]) {
    using namespace Homework;

    Port port;
    std::size_t blockSize;

    try {
        readProgramArguments(argc, argv, port, blockSize);
    } catch (std::invalid_argument& e) {
        std::cerr << e.what() << std::endl;
        return VALIDATION_ERROR;
    }

    boost::asio::io_context ioContext;
    auto commandContext = std::make_shared<CommandProcessingContext>();

    //start listening for 'terminate' signals
    boost::asio::signal_set signals(ioContext, SIGINT, SIGTERM);
    signals.async_wait([&ioContext, commandContext](const boost::system::error_code& error, int signalNumber) {
        if (error) {
            std::cout << "An error occurred in the signal handler: " << error << std::endl;
        } else {
            std::cout << "Terminate signal '" << signalNumber << "' was received." << std::endl;
            commandContext->stop();
            ioContext.stop();
        }

    });

    //start the server
    try {
        commandContext->start(blockSize);

        Server server(ioContext, port, commandContext);

        std::cout << "The server is listening on the port " << port << '.' << std::endl;

        ioContext.run();
    } catch (const std::exception& e) {
        std::cerr << "Internal error: " << e.what() << std::endl;
        return INTERNAL_ERROR;
    }

    return 0;
}
