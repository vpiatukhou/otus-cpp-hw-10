#pragma once

#include <boost/asio.hpp>

#include "CommandProcessingContext.h"

#include <memory>
#include <stdint.h>

namespace Homework {

    using Port = std::uint_least16_t;

    /**
     * An asynchronous TCP server. Creates a new TcpConnection object for an each incoming connection.
     */
    class Server {
    public:
        Server(boost::asio::io_context& ioContext_, Port port_, std::shared_ptr<CommandProcessingContext>& context_);

    private:
        std::shared_ptr<CommandProcessingContext> context;
        boost::asio::ip::tcp::acceptor acceptor;

        void accept();
    };
}