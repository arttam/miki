#include <boost/asio/strand.hpp>

#include "listener.h"
#include "common.h"
#include "session.h"

listener::listener(net::io_context& ioc,
                   ssl::context& ctx,
                   tcp::endpoint endpoint)
    : ioc_(ioc)
    , ctx_(ctx)
    , acceptor_(ioc)
{
    beast::error_code ec;

    // Open the acceptor
    acceptor_.open(endpoint.protocol(), ec);
    if (ec) {

        fail(ec, "open", "listener");
        return;
    }

    // Allow address reuse
    acceptor_.set_option(net::socket_base::reuse_address(true), ec);
    if (ec) {
        fail(ec, "set_option", "listener");
        return;
    }

    // Bind to the server address
    acceptor_.bind(endpoint, ec);
    if (ec) {
        fail(ec, "bind", "listener");
        return;
    }

    // Start listening for connections
    acceptor_.listen(net::socket_base::max_listen_connections, ec);
    if (ec) {
        fail(ec, "listen", "listener");
        return;
    }
}

// Start accepting incoming connections
void listener::run()
{
    do_accept();
}

void listener::do_accept()
{
    // The new connection gets its own strand
    acceptor_.async_accept(
        net::make_strand(ioc_),
        beast::bind_front_handler(
            &listener::on_accept,
            shared_from_this()));
}

void listener::on_accept(beast::error_code ec, tcp::socket socket)
{
    if(ec)
    {
        fail(ec, "accept", "listener");
    }
    else
    {
        std::make_shared<session>(
            std::move(socket),
            ctx_)->run();
    }

    // Accept another connection
    do_accept();
}
