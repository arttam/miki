#ifndef __USES_COMMON_H__
#define __USES_COMMON_H__

#include <boost/beast/core.hpp>

void fail(boost::beast::error_code ec, const char *reason, const char *unit);
void success(const char *which, const char *reason, const char *unit);

#endif
