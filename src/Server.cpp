#include "Server.h"
#include "TcpConnection.h"

#include <boost/asio.hpp>

#include <iostream>

namespace Homework {

    using namespace boost::asio::ip;

    Server::Server(boost::asio::io_context& ioContext_, Port port_, CommandProcessingContext& context_)
        : port(port_), acceptor(ioContext_, tcp::endpoint(tcp::v4(), port)), context(context_) {
    }

    void Server::start() {
        accept();
    }

    void Server::accept() {
        acceptor.async_accept([this](boost::system::error_code error, boost::asio::ip::tcp::socket socket) {
            if (error) {
                std::cerr << "Error accepting request. Code: " << error << " Message: " << error.message() << std::endl;
            } else {
                //Make a shared pointer in order to allow usage enable_shared_from_this in TcpConnection.
                //The instance of TcpConnection won't be deleted when "accept()" returns control because
                //the pointer to it will be "captured" in TcpConnection::listen().
                auto processor = context.createCommandProcessor();
                std::make_shared<TcpConnection>(socket, processor)->listen();

                accept();
            }

            });
    }
}