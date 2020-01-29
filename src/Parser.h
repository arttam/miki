#ifndef __USES_PARSER_H__
#define __USES_PARSER_H__

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
	const std::map<std::string, const char*> mimeType_{{".htm", "text/html"},
	                                                   {".html", "text/html"},
	                                                   {".md", "text/html"},
	                                                   {".php", "text/html"},
	                                                   {".css", "text/css"},
	                                                   {".txt", "text/plain"},
	                                                   {".js", "application/javascript"},
	                                                   {".json", "application/json"},
	                                                   {".xml", "application/xml"},
	                                                   {".swf", "application/x-shockwave-flash"},
	                                                   {".wasm", "application/wasm"},
	                                                   {".flv", "video/x-flv"},
	                                                   {".png", "image/png"},
	                                                   {".jpe", "image/jpeg"},
	                                                   {".jpeg", "image/jpeg"},
	                                                   {".jpg", "image/jpeg"},
	                                                   {".gif", "image/gif"},
	                                                   {".bmp", "image/bmp"},
	                                                   {".ico", "image/vnd.microsoft.icon"},
	                                                   {".tiff", "image/tiff"},
	                                                   {".tif", "image/tiff"},
	                                                   {".svg", "image/svg+xml"},
	                                                   {".svgz", "image/svg+xml"}};

	const std::unordered_set<std::string> cppReserverdWords_{{"alignas"},
	                                                         {"alignof"},
	                                                         {"and"},
	                                                         {"and_eq"},
	                                                         {"asm"},
	                                                         {"atomic_cancel"},
	                                                         {"atomic_commit"},
	                                                         {"atomic_noexcept"},
	                                                         {"auto"},
	                                                         {"bitand"},
	                                                         {"bitor"},
	                                                         {"bool"},
	                                                         {"break"},
	                                                         {"case"},
	                                                         {"catch"},
	                                                         {"char"},
	                                                         {"char8_t"},
	                                                         {"char16_t"},
	                                                         {"char32_t"},
	                                                         {"class"},
	                                                         {"compl"},
	                                                         {"concept"},
	                                                         {"const"},
	                                                         {"consteval"},
	                                                         {"constexpr"},
	                                                         {"constinit"},
	                                                         {"const_cast"},
	                                                         {"continue"},
	                                                         {"co_await"},
	                                                         {"co_return"},
	                                                         {"co_yield"},
	                                                         {"decltype"},
	                                                         {"default"},
	                                                         {"delete"},
	                                                         {"do"},
	                                                         {"double"},
	                                                         {"dynamic_cast"},
	                                                         {"else"},
	                                                         {"enum"},
	                                                         {"explicit"},
	                                                         {"export"},
	                                                         {"extern"},
	                                                         {"false"},
	                                                         {"float"},
	                                                         {"for"},
	                                                         {"friend"},
	                                                         {"goto"},
	                                                         {"if"},
	                                                         {"inline"},
	                                                         {"int"},
	                                                         {"long"},
	                                                         {"mutable"},
	                                                         {"namespace"},
	                                                         {"new"},
	                                                         {"noexcept"},
	                                                         {"not"},
	                                                         {"not_eq"},
	                                                         {"nullptr"},
	                                                         {"operator"},
	                                                         {"or"},
	                                                         {"or_eq"},
	                                                         {"private"},
	                                                         {"protected"},
	                                                         {"public"},
	                                                         {"reflexpr"},
	                                                         {"register"},
	                                                         {"reinterpret_cast"},
	                                                         {"requires"},
	                                                         {"return"},
	                                                         {"short"},
	                                                         {"signed"},
	                                                         {"sizeof"},
	                                                         {"static"},
	                                                         {"static_assert"},
	                                                         {"static_cast"},
	                                                         {"struct"},
	                                                         {"switch"},
	                                                         {"synchronized"},
	                                                         {"template"},
	                                                         {"this"},
	                                                         {"thread_local"},
	                                                         {"throw"},
	                                                         {"true"},
	                                                         {"try"},
	                                                         {"typedef"},
	                                                         {"typeid"},
	                                                         {"typename"},
	                                                         {"union"},
	                                                         {"unsigned"},
	                                                         {"using"},
	                                                         {"virtual"},
	                                                         {"void"},
	                                                         {"volatile"},
	                                                         {"wchar_t"},
	                                                         {"while"},
	                                                         {"xor"},
	                                                         {"xor_eq"},
	                                                         // STL ones
	                                                         {"std::string"}};

public:
	using ResponseType = std::variant<http::response<http::string_body>, http::response<http::empty_body>, http::response<http::file_body>>;

	ResponseType Parse(http::request<http::string_body>&& request);

private:
	http::response<http::string_body> FileTree(std::filesystem::path&& target, const bool keepAlive);
	http::response<http::string_body> MDFile(std::filesystem::path&& target, const bool keepAlive);
	http::response<http::file_body>   FileContents(std::filesystem::path&& target, const bool keepAlive);
	// Edit
	http::response<http::string_body> EditResult(http::request<http::string_body>&& request);

	// Edit helpers
	std::optional<std::string> rmEntry(const std::string& path, const std::string& data) const;
	std::optional<std::string> addEntry(const std::string& path, const std::string& data) const;
	std::optional<std::string> editEntry(const std::string& path, const std::string& data) const;

	// Butify code - my wheel
	void beautifyCode(std::string& payload) const;
};

#endif
