#include <cstdio>
#include <iostream>
#include <string>
#include <algorithm>

#include <emscripten/fetch.h>
#include <emscripten/emscripten.h>

void downloadSucceeded(emscripten_fetch_t *fetch) {
    std::cout << "Finitshed downloadint " << fetch->numBytes << " from "
              << fetch->url << std::endl << std::endl << "Contents:<br /><br />" << std::endl;


    std::string payload(&fetch->data[0], fetch->numBytes);
    std::cout << payload << std::endl;

    emscripten_fetch_close(fetch);
}

void downloadFailed(emscripten_fetch_t *fetch) {
    std::cerr << "Downloadsing of " << fetch->url
              << " failed. Status code: " << fetch->status << std::endl;
    emscripten_fetch_close(fetch);
}

#ifdef __cplusplus
extern "C" {
#endif

void EMSCRIPTEN_KEEPALIVE populateTextArea(int argc, char **argv)
{
    emscripten_fetch_attr_t attr;
    emscripten_fetch_attr_init(&attr);

    strcpy(attr.requestMethod, "GET");
    attr.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY;
    attr.onsuccess = downloadSucceeded;
    attr.onerror = downloadFailed;
    emscripten_fetch(&attr, "/get/");
}

#ifdef __cplusplus
}
#endif

int main() 
{
    std::cout << "ready" << std::endl;

    return 0;
}

