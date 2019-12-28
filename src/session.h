#ifndef __USES_SESSION_H__
#define __USES_SESSION_H__

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/strand.hpp>
#include <boost/config.hpp>

#include <memory>

#include "Parser.h"

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
namespace ssl = boost::asio::ssl;       // from <boost/asio/ssl.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

// Handles an HTTP server connection
class session : public std::enable_shared_from_this<session>
{
    beast::ssl_stream<beast::tcp_stream> stream_;
    beast::flat_buffer buffer_;
    http::request<http::string_body> req_;
    std::shared_ptr<void> res_;

    // Parser
    Parser parser_;
    std::shared_ptr<Parser::ResponseType> responseSP_;

public:
    // Take ownership of the socket
    explicit
    session(
        tcp::socket&& socket,
        ssl::context& ctx);

    // Start the asynchronous operation
    void run();
    void on_handshake(beast::error_code ec);
    void do_read();
    void on_read( beast::error_code ec, std::size_t bytes_transferred);
    void on_write( bool close, beast::error_code ec, std::size_t bytes_transferred);
    void do_close();
    void on_shutdown(beast::error_code ec);
};

#endif
