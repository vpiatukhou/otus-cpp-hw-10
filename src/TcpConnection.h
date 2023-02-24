#pragma once

#include "CommandProcessor.h"
#include "RequestParser.h"

#include <boost/asio.hpp>

#include <array>
#include <cstddef>
#include <cstdint>
#include <memory>

namespace Homework {

    /**
     * A TCP connection. Receives commands from a client and forwards them to CommandProcessingFacade.
     *
     * The connection stays open until the client breaks it.
     */
    class TcpConnection : public std::enable_shared_from_this<TcpConnection> {
    public:
        TcpConnection(boost::asio::ip::tcp::socket& socket_, std::unique_ptr<CommandProcessor>& commandProcessor_);

        void listen();

    private:
        static constexpr std::uint8_t REQUEST_DATA_BLOCK_SIZE_BYTES = 255;

        boost::asio::ip::tcp::socket socket;
        std::unique_ptr<CommandProcessor> commandProcessor;
        RequestParser parser;

        std::array<char, REQUEST_DATA_BLOCK_SIZE_BYTES> request;
    };

}

