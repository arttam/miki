#include "Parser.h"

#include <algorithm>
#include <boost/format.hpp>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <ios>
#include <iterator>
#include <numeric>  // accumulate
#include <optional>
#include <regex>
#include <sstream>
#include <string>
#include <system_error>
#include <unordered_set>
#include <vector>

#include "MimeTypes.h"
#include "ReservedWords.h"

// Temporary
#include <iostream>

#include "../common/resource.h"
#include "cmark-gfm.h"

std::string urlDecode(const std::string target)
{
	std::string percentBuf{};
	std::string response = std::accumulate(
	    target.begin(),
	    target.end(),
	    std::string{},
	    [&percentBuf](std::string response, const char ch) {
		    if (!percentBuf.empty()) {
			    if (percentBuf.length() == 3) {
				    // Try to parse
				    response += static_cast<char>(std::stoi(percentBuf.substr(1), nullptr, 16));
				    percentBuf.clear();
				    // Noow checking current character
				    if (ch == '%') {
					    percentBuf += ch;
				    }
				    else {
					    response += ch;
				    }
			    }
			    else {
				    if (ch >= '0' && ch <= '9') {
					    percentBuf += ch;
				    }
				    else if (ch >= 'a' && ch <= 'f') {
					    percentBuf += ch;
				    }
				    else if (ch >= 'A' && ch <= 'F') {
					    percentBuf += ch;
				    }
				    else {
					    // non hex char - adding everyting including '%' to result
					    response += percentBuf;
					    percentBuf.clear();
				    }
			    }
		    }
		    else if (ch == '%') {
			    percentBuf += ch;
		    }
		    else {
			    response += ch;
		    }
		    return response;
	    });

	if (!percentBuf.empty() && percentBuf.length() == 3) {
		response += static_cast<char>(std::stoi(percentBuf.substr(1), nullptr, 16));
	}

	return response;
}

Parser::ResponseType Parser::Parse(http::request<http::string_body>&& request)
{
	// Returns a bad request response
	auto const bad_request = [&request](const std::string reason) {
		http::response<http::string_body> res{http::status::bad_request, request.version()};
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

	std::string _target = urlDecode(request.target().to_string());

	if (_target.empty() || _target[0] != '/' || _target.find("..") != std::string::npos) {
		return bad_request("Invalid request target");
	}

	if (_target == "/") {
		std::filesystem::path indexFile("miki.html");
		std::error_code       ec;
		if (!std::filesystem::exists(indexFile, ec)) {
			std::string noIndex{"No default file provided: "};
			noIndex.append(ec.message());

			return bad_request(noIndex);
		}

		return FileContents(std::move(indexFile), request.keep_alive());
	}

	// Check if edit command
	if (_target.substr(0, 5) == "/muki") {
		return EditResult(std::move(request));
	}

	// In case of edit - we would add '-raw' suffix to file name
	// And would send it without MD generation
	bool rawMDFile{false};
	if (const auto rawPos = _target.find("-raw", _target.length() - 4, 4);
	    rawPos != std::string::npos) {
		rawMDFile = true;
		_target.erase(rawPos);
	}

	std::filesystem::path pTarget(std::filesystem::relative(std::filesystem::current_path()));

	pTarget += _target;

	std::error_code ec;
	if (!std::filesystem::exists(pTarget, ec)) {
		std::string fileError{"File '"};
		fileError.append(pTarget).append("' does not extist: ").append(ec.message());

		return bad_request(fileError);
	}

	if (std::filesystem::is_regular_file(pTarget)) {
		// Special case
		if (pTarget.extension().string() == ".md" && !rawMDFile) {
			return MDFile(std::move(pTarget), request.keep_alive());
		}
		return FileContents(std::move(pTarget), request.keep_alive());
	}

	// Only directory left at this stage
	return FileTree(std::move(pTarget), request.keep_alive());
}

http::response<http::string_body> Parser::MDFile(std::filesystem::path&& target,
                                                 const bool              keepAlive)
{
	std::ifstream mdFile(target.string());

	std::string asMD;
	asMD.assign(std::istreambuf_iterator<char>(mdFile), std::istreambuf_iterator<char>());

	cmark_parser* parser = cmark_parser_new(CMARK_OPT_DEFAULT);
	cmark_parser_feed(parser, asMD.c_str(), asMD.size());
	cmark_node* document = cmark_parser_finish(parser);

	std::string htmlOut(cmark_render_html(document, CMARK_OPT_DEFAULT, NULL));

	// My changes from here
	beautifyCode(htmlOut);

	cmark_parser_free(parser);
	free(document);

	http::response<http::string_body> resp{};
	resp.result(http::status::ok);
	resp.set(http::field::content_type, "text/markdown");
	resp.keep_alive(keepAlive);
	resp.body() = std::move(htmlOut);
	resp.prepare_payload();

	return resp;
}

http::response<http::string_body> Parser::FileTree(std::filesystem::path&& target,
                                                   const bool              keepAlive)
{
	namespace fs = std::filesystem;
	std::string filesList{};

	if (fs::exists(target)) {
		std::vector<Resource> resources;

		if (fs::is_regular_file(target)) {
			resources.emplace_back(
			    target.filename().string(), target.relative_path().string(), 'p');
		}
		else if (fs::is_directory(target)) {
			for (const fs::directory_entry& de : fs::directory_iterator(target)) {
				resources.emplace_back(de.path().filename().string(),
				                       de.path().relative_path().string().substr(2),
				                       fs::is_directory(de) ? 'd' : 'p');
			}

			std::sort(resources.begin(), resources.end());

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

	http::response<http::string_body> resp{};
	resp.result(http::status::ok);
	resp.set(http::field::content_type, "text/plain");
	resp.keep_alive(keepAlive);
	resp.body() = std::move(filesList);
	resp.prepare_payload();

	return resp;
}

http::response<http::file_body> Parser::FileContents(std::filesystem::path&& target,
                                                     const bool              keepAlive)
{
	// Helper's lamdas
	auto contentType([& MimeTypes = MimeTypes](boost::beast::string_view path) {
		if (const auto pos = path.rfind("."); pos == boost::beast::string_view::npos) {
			return "application/text";
		}
		else {
			std::string ext{path.substr(pos)};
			if (const auto typeIt = MimeTypes.find(ext); typeIt == MimeTypes.end()) {
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

	http::response<http::file_body> res{};
	res.result(http::status::ok);
	res.keep_alive(keepAlive);
	res.set(http::field::server, "Miki");
	res.set(http::field::content_type, contentType(target.string()));
	res.body() = std::move(body);
	res.prepare_payload();

	return res;
}

http::response<http::string_body> Parser::EditResult(http::request<http::string_body>&& request)
{
	// idea: /muki/<COMMAND>/<FULL_PATH_TO_TARGET>
	// where
	// <COMMAND> - [add|rm|edit]
	// <FULL_PATH_TO_TARGET> - full path to target

	// Returns a bad request response
	auto const bad_request = [](const std::string reason) {
		http::response<http::string_body> error{};
		error.result(http::status::bad_request);
		error.set(http::field::server, "Miki");
		error.set(http::field::content_type, "text/html");
		error.keep_alive(false);
		error.body() = reason;
		error.prepare_payload();
		return error;
	};

	std::string editCommand = urlDecode(request.target().to_string());

	// disassembling command
	if (std::count(editCommand.begin(), editCommand.end(), '/') < 3) {
		std::string errorText{"ERROR: Command does not have enough parameters"};
		return bad_request(errorText);
	}

	// removing /muki/
	editCommand.erase(0, 6);
	const auto        commandEnd = editCommand.find_first_of("/");
	const std::string command    = editCommand.substr(0, commandEnd);
	const std::string targetPath = editCommand.substr(commandEnd);
	const std::string payload    = request.body();

	using commandHelper =
	    std::optional<std::string> (Parser::*)(const std::string&, const std::string&) const;
	const std::map<const std::string, commandHelper> commandsMap{
	    {"add", &Parser::addEntry}, {"rm", &Parser::rmEntry}, {"edit", &Parser::editEntry}};

	if (const auto cmdIt = commandsMap.find(command); cmdIt == commandsMap.end()) {
		// Report error
		std::string errorText{"ERROR: Command : '"};
		errorText.append(command).append("' not supported");
		return bad_request(errorText);
	}
	else {
		const auto commandResult = std::invoke(cmdIt->second, this, targetPath, payload);
		if (commandResult.has_value()) {
			std::string errorStr{"Error occurred whilst executing: "};
			errorStr.append(command).append(", Message: ").append(*commandResult);

			return bad_request(errorStr);
		}
		else {
			std::string                       okStr{"ok"};
			http::response<http::string_body> resp{};
			resp.result(http::status::ok);
			resp.set(http::field::content_type, "text/plain");
			resp.keep_alive(request.keep_alive());
			resp.body() = std::move(okStr);
			resp.prepare_payload();

			return resp;
		}
	}
}

std::optional<std::string> Parser::rmEntry(const std::string& path, const std::string&) const
{
	// Remove entry, if not exists - return error
	std::filesystem::path pTarget(std::filesystem::relative(std::filesystem::current_path()));
	pTarget += path;

	if (std::filesystem::exists(pTarget)) {
		std::error_code ec;
		if (!std::filesystem::remove(pTarget, ec)) {
			return (boost::format("Failed to remove '%1%', error message: '%2%'") % path %
			        ec.message())
			    .str();
		}
	}
	else {
		return (boost::format("Remove candidate '%1%' not found") % path).str();
	}

	return {};
}

std::optional<std::string> Parser::addEntry(const std::string& path, const std::string& data) const
{
	// Add entry, if wrong path - return error
	std::filesystem::path pTarget(std::filesystem::relative(std::filesystem::current_path()));
	pTarget += path;

	std::filesystem::path pTargetDirectory = pTarget;
	pTargetDirectory.remove_filename();

	std::error_code ec;
	if (!std::filesystem::create_directories(pTargetDirectory, ec) && ec.value() != 0) {
		return (boost::format("Failed create destination directory '%1%', error message: '%2%'") %
		        pTargetDirectory.string() % ec.message())
		    .str();
	}

	std::ofstream addedEntry(pTarget.string(), std::ios::out);
	if (!addedEntry) {
		return (boost::format("Failed to open '%1%' for writting") % pTarget.string()).str();
	}

	addedEntry.write(data.c_str(), data.length());
	addedEntry.flush();
	addedEntry.close();

	return {};
}

std::optional<std::string> Parser::editEntry(const std::string& path, const std::string& data) const
{
	// Replace entry, if wrong path - return error
	std::filesystem::path pTarget(std::filesystem::relative(std::filesystem::current_path()));
	pTarget += path;

	if (!std::filesystem::exists(pTarget)) {
		return (boost::format("Failed to find '%1%' for edit") % pTarget).str();
	}

	std::ofstream edittedEntry(pTarget.string(), std::ios::out | std::ios::trunc);
	if (!edittedEntry) {
		return (boost::format("Failed to open '%1%' for writting") % pTarget).str();
	}

	edittedEntry.write(data.c_str(), data.length());
	edittedEntry.flush();
	edittedEntry.close();

	return {};
}

void Parser::beautifyCode(std::string& payload) const
{
	const std::string codeTagOpen  = R"(<code class="language-cpp">)";
	const std::string codeTagClose = R"(</code>)";

	const std::unordered_set<std::string> validPreStrs{
	    "::", " ", "\n", "\t", "&lt;", "&gt;", "*", "&amp;", "(", ")", "{"};

	const std::unordered_set<std::string> validPostStrs{
	    "::", ";", " ", "\n", "\t", "&lt;", "&gt;", "*", "&amp;", "(", ")", ","};

	auto snippetPos = payload.find(codeTagOpen);
	while (snippetPos != std::string::npos) {
		auto snippetTo = payload.find(codeTagClose, snippetPos + 1);
		if (snippetTo == std::string::npos) {
			std::cerr << "Should not happen at all - no closing </code> tag, leaving" << std::endl;
			return;
		}

		const auto  formatFrom = snippetPos + codeTagOpen.length();
		const auto  formatLen  = snippetTo - formatFrom;
		std::string formatMe   = payload.substr(formatFrom, formatLen);

		const auto validResvWord(
		    [&validPreStrs, &validPostStrs, &formatMe](const size_t pos, const std::string& word) {
			    // Prefix first
			    // If starts with some chars before reserved word
			    if (pos > 0) {
				    bool prefixValid{false};

				    for (const auto& validSet : validPreStrs) {
					    if (validSet.size() > pos)
						    continue;

					    if (formatMe.substr(pos - validSet.size(), validSet.size()) == validSet) {
						    prefixValid = true;
						    break;
					    }
				    }

				    if (!prefixValid)
					    return false;
			    }

			    bool suffixValid{false};
			    for (const auto& validSet : validPostStrs) {
				    if (pos + word.size() + validSet.size() > formatMe.size())
					    continue;

				    if (formatMe.substr(pos + word.size(), validSet.size()) == validSet) {
					    suffixValid = true;
					    break;
				    }
			    }
			    return suffixValid;
		    });

		bool needReplacement = false;
		for (const auto& [tag, resvwords] : CPPFormatting) {
			for (const auto& resvW : resvwords) {
				// TODO: Skip already wrapped in span tag ???
				auto resvWPos = formatMe.find(resvW);
				while (resvWPos != std::string::npos) {
					if (validResvWord(resvWPos, resvW)) {
						needReplacement = true;

						std::stringstream istr;
						istr << "<span class=\"" << tag << "\">" << resvW << "</span>";

						formatMe.replace(resvWPos, resvW.length(), istr.str());
						resvWPos += istr.str().length();

						istr.str("");
					}
					else {
						resvWPos += resvW.length();
					}
					resvWPos = formatMe.find(resvW, resvWPos);
				}
			}
		}

		// TODO
		// Check for #include directives - and amend accordingly

		// Check for comments - and amend accordingly

		if (needReplacement) {
			payload.replace(formatFrom, formatLen, formatMe);
			snippetTo = formatFrom + formatMe.length();
		}

		// snippedTo must me amended if changes were done
		snippetPos = payload.find(codeTagOpen, snippetTo);
	}
}
