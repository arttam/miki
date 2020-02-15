#pragma once

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <filesystem>
#include <map>
#include <optional>
#include <string>
#include <unordered_set>
#include <variant>

namespace http = boost::beast::http;  // from <boost/beast/http.hpp>

class Parser {
public:
	using ResponseType = std::variant<http::response<http::string_body>,
	                                  http::response<http::empty_body>,
	                                  http::response<http::file_body>>;

	ResponseType Parse(http::request<http::string_body>&& request);

private:
	http::response<http::string_body> FileTree(std::filesystem::path&& target,
	                                           const bool              keepAlive);
	http::response<http::string_body> MDFile(std::filesystem::path&& target, const bool keepAlive);
	http::response<http::file_body>   FileContents(std::filesystem::path&& target,
	                                               const bool              keepAlive);
	// Edit
	http::response<http::string_body> EditResult(http::request<http::string_body>&& request);

	// Edit helpers
	std::optional<std::string> rmEntry(const std::string& path, const std::string& data) const;
	std::optional<std::string> addEntry(const std::string& path, const std::string& data) const;
	std::optional<std::string> editEntry(const std::string& path, const std::string& data) const;

	// Butify code - my wheel
	void beautifyCode(std::string& payload) const;
};

