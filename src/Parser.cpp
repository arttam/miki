#include "Parser.h"
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <vector>
#include <algorithm>
#include <fstream>

// Temporary
#include <iostream>

#include "../common/resource.h"

#include "cmark-gfm.h"

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
    std::cerr << "Got target as: " << _target << std::endl;

    if (_target.empty() || _target[0] != '/' ||
        _target.find("..") != std::string::npos)
    {
        return bad_request("Invalid request target");
    }

    if (_target == "/") {
        boost::filesystem::path indexFile("miki.html");
        if (!boost::filesystem::exists(indexFile)) {
            std::string noIndex{"No default file provided"};

            return bad_request(noIndex);
        }

        return FileContents(std::move(indexFile), request.keep_alive());
    }

    _target.insert(0, docRoot);

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
        // Special case
        if (pTarget.extension().string() == ".md")
        {
            return MDFile(std::move(pTarget), request.keep_alive());
        }
        return FileContents(std::move(pTarget), request.keep_alive());
    }

    // Only directory left at this stage
    return FileTree(std::move(pTarget), request.keep_alive());
}

http::response<http::string_body> Parser::MDFile(boost::filesystem::path&& target, const bool keepAlive)
{
    std::ifstream mdFile(target.string());

    std::string asMD;
    asMD.assign(std::istreambuf_iterator<char>(mdFile),
            std::istreambuf_iterator<char>());

    cmark_parser *parser = cmark_parser_new(CMARK_OPT_DEFAULT);
    cmark_parser_feed(parser, asMD.c_str(), asMD.size());
    cmark_node *document = cmark_parser_finish(parser);

    std::string htmlOut(cmark_render_html(document, CMARK_OPT_DEFAULT, NULL));

    cmark_parser_free(parser);
    free(document);

    http::response<http::string_body> resp {};
    resp.result(http::status::ok);
    resp.set(http::field::content_type, "text/markdown");
    resp.keep_alive(keepAlive);
    resp.body() = std::move(htmlOut);
    resp.prepare_payload();

    return resp;
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
                   de.path().relative_path().string().substr(2),
                   boost::filesystem::is_directory(de) ? 'd' : 'p');
            }

            for (const auto& resouce : resources) {
                filesList.append(resouce.asString().append(";"));
            }
        }
        else {
            // TODO: Decide what to do with errors
        }
    }
    else {
        // TODO: Decide what to do with errors
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
