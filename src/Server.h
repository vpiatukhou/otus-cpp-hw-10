#pragma once

#include <boost/asio.hpp>

#include "CommandProcessingContext.h"

#include <stdint.h>

namespace Homework {

    using Port = std::uint_least16_t;

    /**
     * An asynchronous TCP server. Creates a new TcpConnection object for an each incoming connection.
     */
    class Server {
    public:
        Server(boost::asio::io_context& ioContext_, Port port_, CommandProcessingContext& context_);

        void start();

    private:
        Port port;
        boost::asio::ip::tcp::acceptor acceptor;
        CommandProcessingContext& context;

        void accept();
    };
}