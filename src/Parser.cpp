#include <filesystem>
#include <boost/format.hpp>
#include <iterator>
#include <string>
#include <system_error>
#include <vector>
#include <algorithm>
#include <fstream>
#include <regex>
#include <filesystem>

#include "Parser.h"

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
        std::filesystem::path indexFile("miki.html");
        std::error_code ec;
        if (!std::filesystem::exists(indexFile, ec))
        {
            std::string noIndex{"No default file provided: "};
            noIndex.append(ec.message());

            return bad_request(noIndex);
        }

        return FileContents(std::move(indexFile), request.keep_alive());
    }

    // Check if edit command
    if (_target.substr(0, 5) == "/muki") {
        return EditResult(std::move(_target), request.keep_alive());
    }

    std::filesystem::path pTarget(std::filesystem::relative(std::filesystem::current_path()));

    pTarget += _target;

    std::cerr
        << (boost::format("Working with: %1%") % pTarget.string()).str()
        << std::endl;

    std::error_code ec;
    if (!std::filesystem::exists(pTarget, ec))
    {
        std::string fileError{"File '"};
        fileError
            .append(pTarget)
            .append("' does not extist: ")
            .append(ec.message());

        return bad_request(fileError);
    }

    if (std::filesystem::is_regular_file(pTarget))
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

http::response<http::string_body> Parser::MDFile(std::filesystem::path&& target, const bool keepAlive)
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

http::response<http::string_body> Parser::FileTree(std::filesystem::path&& target, const bool keepAlive)
{
    std::string filesList {};

    if (std::filesystem::exists(target)) {
        std::vector<Resource> resources;

        if (std::filesystem::is_regular_file(target)) {
            resources.emplace_back(
                target.filename().string(),
                target.relative_path().string(),
                'p');
        }
        else if (std::filesystem::is_directory(target)) {
            for (const std::filesystem::directory_entry& de :
                 std::filesystem::directory_iterator(target)) {

                resources.emplace_back(
                   de.path().filename().string(),
                   de.path().relative_path().string().substr(2),
                   std::filesystem::is_directory(de) ? 'd' : 'p');
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

http::response<http::file_body> Parser::FileContents(std::filesystem::path&& target, const bool keepAlive)
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

http::response<http::string_body> Parser::EditResult(std::string&& editCommand, const bool keepAlive)
{
    // idea: /muki/<COMMAND>/<FULL_PATH_TO_TARGET>
    // where
    // <COMMAND> - [add|rm|edit]
    // <FULL_PATH_TO_TARGET> - full path to target

    std::regex separator("/");
    std::sregex_token_iterator reIt(editCommand.begin(), editCommand.end(), separator, -1);
    std::sregex_token_iterator reEnd{};
    std::vector<std::string> commandParts;
    std::copy_if(reIt, reEnd,
        std::back_inserter(commandParts),
        [](const auto& part)
        {
            return !(part.str().empty());
        });

    // Returns a bad request response
    auto const bad_request = [](const std::string reason) {
        http::response<http::string_body> error {};
        error.result(http::status::bad_request);
        error.set(http::field::server, "Miki");
        error.set(http::field::content_type, "text/html");
        error.keep_alive(false);
        error.body() = reason;
        error.prepare_payload();
        return error;
    };

    if (commandParts.size() < 3)
    {
        std::string errorText{"ERROR: Command does not have enough parameters"};
        return bad_request(errorText);
    }

    enum class commandIdx: unsigned { MUKI=0, COMMAND, START_OF_PATH };
    const std::vector<std::string> validCommands{"add", "rm", "edit"};
    const auto cmdIt = std::find(validCommands.begin(), validCommands.end(), commandParts[static_cast<size_t>(commandIdx::COMMAND)]);
    if (cmdIt == validCommands.end())
    {
        // Report error
        std::string errorText{"ERROR: Command : '"};
        errorText.append(commandParts[static_cast<unsigned>(commandIdx::COMMAND)]).append("' not supported");
        return bad_request(errorText);
    }

    // Debug output
    std::string editResponse;
    for (const auto& cmdPart : commandParts)
    {
        editResponse.append(cmdPart).append("\r\n");
    }
    editResponse.append("\r\n");

    for (auto idx=0; idx < commandParts.size(); ++idx)
    {
        editResponse.append(std::to_string(idx)).append(" => '").append(commandParts[idx]).append("'\r\n");
    }
    editResponse.append("\r\n");

    editResponse.append("MUKI: ").append(std::to_string(static_cast<unsigned>(commandIdx::MUKI))).append("\r\n");
    editResponse.append("COMMAND: ").append(std::to_string(static_cast<unsigned>(commandIdx::COMMAND))).append("\r\n");
    editResponse.append("START_OF_PATH: ").append(std::to_string(static_cast<unsigned>(commandIdx::START_OF_PATH))).append("\r\n");
    editResponse.append("\r\n");

    editResponse.append("Command: ").append(commandParts[static_cast<unsigned>(commandIdx::COMMAND)]).append("\r\n");

    http::response<http::string_body> resp{};
    resp.result(http::status::ok);
    resp.set(http::field::content_type, "text/plain");
    resp.keep_alive(keepAlive);
    resp.body() = std::move(editResponse);
    resp.prepare_payload();

    return resp;
}
