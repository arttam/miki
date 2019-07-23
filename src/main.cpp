#include <iostream>
#include <string>
#include <memory>
#include <thread>
#include <vector>
#include <functional>
#include <map>

#include <boost/asio.hpp>

int main(int argc, char** argv)
{
    std::string address{"0.0.0.0"};
    unsigned short port{8080};
    std::string docRoot{"."};
    unsigned int threads = std::thread::hardware_concurrency();

    using pIt = std::vector<std::string>::const_iterator;
    std::map<std::string, std::function<void(pIt& paramsIt)>> paramsMap
    {
          {"--host",     ([&](pIt& paramIt){ address = *++paramIt; })}
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

    std::cout << "Params now as follows:\n"
        << "Host: " << address
        << "; Port: " << port
        << "; DocRoot: " << docRoot
        << "; Thread: " << threads;

    return 0;
}
