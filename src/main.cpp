#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/strand.hpp>
#include <boost/config.hpp>

#include <iostream>
#include <string>
#include <memory>
#include <thread>
#include <vector>
#include <functional>
#include <map>

#include <boost/format.hpp>

#include "server_certificate.hpp"
#include "listener.h"

std::string docRoot{"."};

int main(int argc, char** argv)
{
    std::string addressStr{"0.0.0.0"};
    unsigned short port{8080};
    int threads = std::thread::hardware_concurrency();

    using pIt = std::vector<std::string>::const_iterator;
    std::map<std::string, std::function<void(pIt& paramsIt)>> paramsMap
    {
          {"--host",     ([&](pIt& paramIt){ addressStr = *++paramIt; })}
        , {"--port",     ([&](pIt& paramIt){ port = std::stoi(*++paramIt); })}
        , {"--doc-root", ([&](pIt& paramIt){ docRoot = *++paramIt; })}
        , {"--threads",  ([&](pIt& paramIt){ threads = std::stoi(*++paramIt); })}
    };

    if (argc > 1)
    {
        std::vector<std::string> args(argv, argv + argc);
        std::vector<std::string>::const_iterator argsIt = args.begin();

        while (argsIt != args.end())
        {
            if (const auto& pIt = paramsMap.find(*argsIt);
                    pIt != paramsMap.end())
            {
                pIt->second(argsIt);
            }
            ++argsIt;
        }
    }
    
    std::cout 
        << (boost::format("Starting server as: host='%1%', port=%2%, doc-root='%3%', number of threads: %4%") 
                % addressStr
                % port
                % docRoot
                % threads).str()
        << std::endl;

    const auto  address = boost::beast::net::ip::make_address(addressStr);
    boost::beast::net::io_context ioc{threads};

    boost::asio::ssl::context ctx{boost::asio::ssl::context::tlsv12};
    load_server_certificate(ctx);

    std::make_shared<listener>(
        ioc, 
        ctx, 
        boost::asio::ip::tcp::endpoint{address, port}, 
        std::make_shared<std::string>(docRoot))->run();
    
    std::vector<std::thread> v;
    v.reserve(threads - 1);
    for (auto i = threads - 1; i > 0; --i)
    {
        v.emplace_back(
            [&ioc]()
            {
                ioc.run();
            });
    }
    ioc.run();

    return 0;
};
