#include "common.h"

#include <iostream>
#include <boost/format.hpp>

void fail(boost::beast::error_code ec, const char *reason, const char *unit)
{
    std::cerr << 
        (boost::format("Error in [%1%], Reason: '%2%', Message: ") 
            % unit
            % reason
            % ec.message()).str() << std::endl;
}

void success(const char *which, const char *reason, const char *unit)
{
    std::cerr << 
        (boost::format("Success!!! '%1%' in [%2%], Mothod: '%3%'") 
            % which
            % unit
            % reason).str() << std::endl;
}
