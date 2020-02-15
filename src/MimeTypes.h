#pragma once
#include <string>
#include <unordered_map>

const std::unordered_map<std::string, const char*> MimeTypes{
    {".htm", "text/html"},
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
