#include "Server.h"

#include <boost/asio.hpp>

#include <csignal>
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

        const std::string INVALID_PORT = "A port must be in the range [0-" + std::to_string(MAX_PORT) + "].";

        //we have to make the context a global variable because it is used in cleanUp()
        CommandProcessingContext commandContext;
    }

    Port stringToPort(const std::string& portAsStr) {
        long number = std::stoul(portAsStr);
        if (number > MAX_PORT) {
            throw std::invalid_argument(INVALID_PORT);
        }
        return static_cast<Port>(number);
    }

    void readProgramArguments(int argc, char* argv[], Port& outputPort, std::size_t& outputBlockSize) {
        if (argc != NUMBER_OF_ARGUMENTS) {
            throw std::invalid_argument("Wrong number of arguments. The format of the command is:\nbulk_server port block_size");
        }

        outputPort = stringToPort(argv[PORT_ARG_INDEX]);
        outputBlockSize = static_cast<std::size_t>(std::stoul(argv[BLOCK_SIZE_ARG_INDEX]));
    }

    void processShutdownHandler(int signal) {
        commandContext.stop();
        exit(signal);
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

    //the program must finish its work if "terminate" signal has come.
    signal(SIGTERM, &processShutdownHandler);
    signal(SIGINT, &processShutdownHandler);

    commandContext.init(blockSize);

    //start up the server
    try {
        boost::asio::io_context ioContext;

        Server server(ioContext, port, commandContext);
        server.start();

        std::cout << "The server is listening on the port " << port << '.' << std::endl;

        ioContext.run();
    } catch (const std::exception& e) {
        std::cerr << "Unexpected error occurred: " << e.what() << std::endl;
    }

    return 0;
}
