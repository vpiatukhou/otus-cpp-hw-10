#include "TcpConnection.h"

#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>

namespace Homework {

    TcpConnection::TcpConnection(boost::asio::ip::tcp::socket& socket_, std::unique_ptr<CommandProcessor>& commandProcessor_) :
        socket(std::move(socket_)), 
        commandProcessor(std::move(commandProcessor_)) {

        parser.onCommandReceived = [&commandProcessor = this->commandProcessor](const std::string& command) {
            try {
                commandProcessor->process(command);
            } catch (std::exception& e) {
                std::cerr << "Error processing request: " << e.what() << std::endl;
            }
        };
    }

    void TcpConnection::listen() {
        //pass "self" to the callback in order to keep the instance of TcpConnection alive while the connection exists
        auto self = shared_from_this();
        auto inputHandler = [this, self](boost::system::error_code error, std::size_t bytesTransferred) {
            if (error) {
                //if the connection hasn't been closed by the client, print an error
                if (error != boost::asio::error::eof && error != boost::asio::error::connection_reset) {
                    std::cerr << "Error reading incoming data. Code: " << error << " Message: " << error.message() << std::endl;
                }
            } else {
                if (bytesTransferred > 0) {
                    std::string toParse(request.data(), bytesTransferred);
                    try {
                        parser.parse(toParse);
                    } catch (std::exception& e) {
                        std::cerr << "Error parsing '" << toParse << "': " << e.what() << std::endl;
                    }
                }

                listen();
            }
        };

        socket.async_read_some(boost::asio::buffer(request, REQUEST_DATA_BLOCK_SIZE_BYTES), inputHandler);
    }
}

