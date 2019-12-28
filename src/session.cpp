#include "session.h"
#include "common.h"

session::session(tcp::socket&& socket,
                 ssl::context& ctx)
    : stream_(std::move(socket), ctx)
{}

// Start the asynchronous operation
void session::run()
{
    // Set the timeout.
    beast::get_lowest_layer(stream_).expires_after(std::chrono::seconds(30));

    // Perform the SSL handshake
    stream_.async_handshake(ssl::stream_base::server,
                            beast::bind_front_handler(&session::on_handshake, shared_from_this()));
}

void session::on_handshake(beast::error_code ec)
{
    if (ec)
        return fail(ec, "handshake", "session");

    do_read();
}

void session::do_read()
{
    // Make the request empty before reading,
    // otherwise the operation behavior is undefined.
    req_ = {};

    // Set the timeout.
    beast::get_lowest_layer(stream_).expires_after(std::chrono::seconds(30));

    // Read a request
    http::async_read(
        stream_, 
        buffer_, 
        req_, 
        beast::bind_front_handler(&session::on_read, shared_from_this()));
}

void session::on_read(beast::error_code ec, std::size_t bytes_transferred)
{
    boost::ignore_unused(bytes_transferred);

    // This means they closed the connection
    if (ec == http::error::end_of_stream)
        return do_close();

    if (ec)
        return fail(ec, "read", "session");

    responseSP_ = std::make_shared<Parser::ResponseType>(parser_.Parse(std::move(req_)));
    switch (responseSP_->index()) {
        case 0: 
            {
                http::response<http::string_body>* payload = 
                    &std::get<http::response<http::string_body>>(*responseSP_);

                http::async_write(
                    stream_,
                    *payload,
                    beast::bind_front_handler(
                        &session::on_write,
                        shared_from_this(),
                        false));
            } 
            break;
        case 1: 
            {
                auto* payload = &std::get<http::response<http::empty_body>>(*responseSP_);
                
                http::async_write(
                    stream_,
                    *payload,
                    beast::bind_front_handler(
                        &session::on_write,
                        shared_from_this(),
                        false));
            
            } 
            break;
        case 2: 
            {
                auto* payload = &std::get<http::response<http::file_body>>(*responseSP_);

                http::async_write(
                    stream_,
                    *payload,
                    beast::bind_front_handler(
                        &session::on_write,
                        shared_from_this(),
                        false));

            } 
            break;
    }
}

void session::on_write(bool close, beast::error_code ec, std::size_t bytes_transferred)
{
    boost::ignore_unused(bytes_transferred);

    if (ec)
        return fail(ec, "write", "session");

    if (close) {
        // This means we should close the connection, usually because
        // the response indicated the "Connection: close" semantic.
        return do_close();
    }

    // We're done with the response so delete it
    res_ = nullptr;

    // Read another request
    do_read();
}

void session::do_close()
{
    // Set the timeout.
    beast::get_lowest_layer(stream_).expires_after(std::chrono::seconds(30));

    // Perform the SSL shutdown
    stream_.async_shutdown(beast::bind_front_handler(&session::on_shutdown, shared_from_this()));
}

void session::on_shutdown(beast::error_code ec)
{
    if (ec)
        return fail(ec, "shutdown", "session");

    // At this point the connection is closed gracefully
}
