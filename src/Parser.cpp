#include "Parser.h"
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <vector>
#include <algorithm>

// Temporary
#include <iostream>

#include "../common/resource.h"

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

    if (request.method() != http::verb::get && request.method() != http::verb::post) {
        return bad_request("Method is not supported");
    }

    std::string _target = request.target().to_string();

    if (_target.empty() || _target[0] != '/' ||
        _target.find("..") != std::string::npos)
    {
        return bad_request("Invalid request target");
    }

    if (_target == "/") {
        boost::filesystem::path indexFile("index.html");
        if (!boost::filesystem::exists(indexFile)) {
            std::string noIndex{"No default file provided"};

            return bad_request(noIndex);
        }

        return FileContents(std::move(indexFile), request.keep_alive());
    }

    const std::string browsePrefix{"/get/"};
    if (_target.substr(0, browsePrefix.length()) == browsePrefix) {
        _target = _target.replace(0, browsePrefix.length(), docRoot);

        boost::filesystem::path pTarget(_target);
        if (!boost::filesystem::exists(pTarget)) {
            std::string fileErr {"File ''"};
            fileErr.append(_target).append("' does not exists");

            return bad_request(fileErr);
        }
        std::cerr
            << (boost::format("Working with: %1%") % pTarget.relative_path().string()).str()
            << std::endl;

        if (boost::filesystem::is_regular_file(pTarget))
        {
            return FileContents(std::move(pTarget), request.keep_alive());
        }

        // Only directory left at this stage
        return FileTree(std::move(pTarget), request.keep_alive());
    }

    std::string error {"Not implemented yet: "};
    error.append(std::string(_target));
    return bad_request(error);
}

http::response<http::string_body> Parser::FileTree(boost::filesystem::path&& target, const bool keepAlive)
{
    std::string filesList {};

    if (boost::filesystem::exists(target)) {
        std::vector<Resource> resources;

        if (boost::filesystem::is_regular_file(target)) {
            resources.emplace_back(
                target.filename().string(), 
                target.relative_path().string(),
                'p');
        }
        else if (boost::filesystem::is_directory(target)) {
            for (boost::filesystem::directory_entry& de :
                 boost::filesystem::directory_iterator(target)) {

                resources.emplace_back(
                   de.path().filename().string(),
                   de.path().relative_path().string(),
                   boost::filesystem::is_directory(de) ? 'd' : 'p');
            }

            for (const auto& resouce : resources) {
                filesList.append(resouce.asString().append(";"));
            }
        }
        else {
            // TODO: Decide what to do with errors
            /*
            filesList += "<h2>Path: <b>";
            filesList += target.string();
            filesList += "</b> is neither file nor directory</h2>";
            */
        }
    }
    else {
        // TODO: Decide what to do with errors
        /*
        filesList += "<h2>Path: <b>";
        filesList += target.string();
        filesList += "</b> does not exists</h2>";
        */
    }

    http::response<http::string_body> resp {};
    resp.result(http::status::ok);
    resp.set(http::field::content_type, "text/plain");
    resp.keep_alive(keepAlive);
    resp.body() = std::move(filesList);
    resp.prepare_payload();

    return resp;
}

http::response<http::file_body> Parser::FileContents(boost::filesystem::path&& target, const bool keepAlive)
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
    body.open(target.string().c_str(), boost::beast::file_mode::scan, ec);

    http::response<http::file_body> res {};
    res.result(http::status::ok);
    res.keep_alive(keepAlive);
    res.set(http::field::server, "Miki");
    res.set(http::field::content_type, contentType(target.string()));
    res.body() = std::move(body);
    res.prepare_payload();

    return res;
}
