#include "Parser.h"
#include <boost/filesystem.hpp>
#include <vector>
#include <algorithm>

// Temporary
#include <iostream>

extern std::string docRoot;

Parser::ResponseType Parser::Parse(http::request<http::string_body>&& request)
{

    // Returns a bad request response
    auto const bad_request = [&request](const std::string reason) {
        http::response<http::string_body> res {http::status::bad_request, request.version()};
        res.set(http::field::server, "Miki");
        res.set(http::field::content_type, "text/html");
        res.keep_alive(request.keep_alive());
        res.body() = reason;
        res.prepare_payload();
        return res;
    };

    auto const check_file(
        []
        (const std::string& filePath)
        {
            
        });

    if (request.method() != http::verb::get && request.method() != http::verb::post) {
        return bad_request("Method is not supported");
    }

    if (request.target().empty() || request.target()[0] != '/' ||
        request.target().find("..") != boost::beast::string_view::npos) {
        return bad_request("Invalid request target");
    }

    if (request.target() == "/tree")
    {
        return FileTree(std::move(request));
    }

    if (request.target().substr(0, 6) == "/file/")
    {
        std::string fileFullPath(request.target().substr(6));
        if (!boost::filesystem::exists(boost::filesystem::path(fileFullPath)))
        {
            std::string fileErr{"File ''"};
            fileErr.append(fileFullPath).append("' does not exists");

            return bad_request(fileErr);
        }

        return FileContents(std::move(request));
    }
    
    std::string error{"Not implemented yet: "};
    error.append(std::string(request.target()));
    return bad_request(error);
}

http::response<http::string_body> Parser::FileTree(http::request<http::string_body>&& request)
{
    std::string filesList {"<html><title>"};
    filesList.append(std::string(request.target()));
    filesList += "</title><head>";
    filesList += docRoot;
    filesList += "</head><body>";

    boost::filesystem::path rp(docRoot);
    if (boost::filesystem::exists(rp)) {
        std::vector<std::pair<std::string, int>> entries;
        if (boost::filesystem::is_regular_file(rp))
        {
            entries.emplace_back(rp.string(), 1);
        }
        else if(boost::filesystem::is_directory(rp))
        {
            for (boost::filesystem::directory_entry& de : boost::filesystem::directory_iterator(rp))
            {
                entries.emplace_back(
                    de.path().filename().string(), 
                    boost::filesystem::is_regular_file(de) ? 1 : 0);
            }

            using elType = decltype(entries.front());
            std::sort(entries.begin(), entries.end(), [](elType& lhs, elType& rhs) {
                if (lhs.second == rhs.second) {
                    return (lhs.first < rhs.first);
                }
                return (lhs.second < rhs.second);
            });

            filesList += "<ul>";
            for (const auto& entry : entries) {
                const auto& [name, weight] = entry;
                filesList += "<li>";
                filesList += (weight == 0 ? "[" : "<i>");
                filesList += name;
                filesList += (weight == 0 ? "]" : "</i>");
            }
        }
        else {
            filesList += "<h2>Path: <b>";
            filesList += std::string(request.target());
            filesList += "</b> is neither file nor directory</h2>";
        }
    }
    else {
        filesList += "<h2>Path: <b>";
        filesList += std::string(request.target());
        filesList += "</b> does not exists</h2>";
    }

    filesList += "</body></html>";

    http::response<http::string_body> resp {};
    resp.result(http::status::ok);
    resp.set(http::field::content_type, "text/html");
    resp.keep_alive(request.keep_alive());
    resp.body() = std::move(filesList);
    resp.prepare_payload();

    return resp;
}

http::response<http::file_body> Parser::FileContents(http::request<http::string_body>&& request)
{
    // Helper's lamdas
    auto contentType([& mimeType_ = mimeType_](boost::beast::string_view path) {
        if (const auto pos = path.rfind("."); pos == boost::beast::string_view::npos) {
            return "application/text";
        }
        else {
            std::string ext {path.substr(pos)};
            if (const auto typeIt = mimeType_.find(ext); typeIt == mimeType_.end()) {
                return "application/text";
            }
            else {
                return typeIt->second;
            }
        }
    });

    http::file_body::value_type body;

    boost::beast::error_code ec;

    // At this point checks were made if file actually exists
    body.open(request.target().substr(6).data(), boost::beast::file_mode::scan, ec);

    http::response<http::file_body> res {};
    res.result(http::status::ok);
    res.keep_alive(request.keep_alive());
    res.set(http::field::server, "Miki");
    res.set(http::field::content_type, contentType(request.target()));
    res.body() = std::move(body);
    res.prepare_payload();

    return res;
}
