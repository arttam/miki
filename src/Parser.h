#ifndef __USES_PARSER_H__
#define __USES_PARSER_H__

#include <variant>
#include <map>

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <filesystem>
#include <optional>

namespace http = boost::beast::http; // from <boost/beast/http.hpp>

class Parser
{
    const std::map<std::string, const char*> mimeType_ {{".htm",  "text/html"},
                                                        {".html", "text/html"},
                                                        {".md",   "text/html"},
                                                        {".php",  "text/html"},
                                                        {".css",  "text/css"},
                                                        {".txt",  "text/plain"},
                                                        {".js",   "application/javascript"},
                                                        {".json", "application/json"},
                                                        {".xml",  "application/xml"},
                                                        {".swf",  "application/x-shockwave-flash"},
                                                        {".wasm", "application/wasm"},
                                                        {".flv",  "video/x-flv"},
                                                        {".png",  "image/png"},
                                                        {".jpe",  "image/jpeg"},
                                                        {".jpeg", "image/jpeg"},
                                                        {".jpg",  "image/jpeg"},
                                                        {".gif",  "image/gif"},
                                                        {".bmp",  "image/bmp"},
                                                        {".ico",  "image/vnd.microsoft.icon"},
                                                        {".tiff", "image/tiff"},
                                                        {".tif",  "image/tiff"},
                                                        {".svg",  "image/svg+xml"},
                                                        {".svgz", "image/svg+xml"}};

public:
    using ResponseType = std::variant<http::response<http::string_body>,
                                      http::response<http::empty_body>,
                                      http::response<http::file_body>>;

    ResponseType Parse(http::request<http::string_body>&& request);

private:
    http::response<http::string_body> FileTree(std::filesystem::path&& target, const bool keepAlive);
    http::response<http::string_body> MDFile(std::filesystem::path&& target, const bool keepAlive);
    http::response<http::file_body> FileContents(std::filesystem::path&& target, const bool keepAlive);
    // Edit
    http::response<http::string_body> EditResult(http::request<http::string_body>&& request);

    // Edit helpers
    std::optional<std::string> rmEntry(const std::string& path, const std::string& data) const;
    std::optional<std::string> addEntry(const std::string& path, const std::string& data) const;
    std::optional<std::string> editEntry(const std::string& path, const std::string& data) const;
};

#endif
