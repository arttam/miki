#include <cstdio>
#include <iostream>
#include <string>
#include <algorithm>
#include <functional>

#include <emscripten/fetch.h>
#include <emscripten/emscripten.h>
#include <emscripten/bind.h>

using namespace emscripten;

struct MikiEntry
{
    std::string name;
    std::string path;
    char type;
};

std::function<void(emscripten_fetch_t *attr)> downloadSucceededImpl;
void DownloadSucceeded(emscripten_fetch_t *attr)
{
    downloadSucceededImpl(attr);
}
std::function<void(emscripten_fetch_t *attr)> downloadFailedImpl;
void DownloadFailed(emscripten_fetch_t *attr)
{
    downloadFailedImpl(attr);
}

class Accessor
{
public:
    Accessor()
    {
        downloadSucceededImpl = std::bind(&Accessor::OnSuceess, this, std::placeholders::_1);
        downloadFailedImpl    = std::bind(&Accessor::OnError, this, std::placeholders::_1);
    }
    virtual ~Accessor() {}

    // Request
    void get(const std::string url)
    {
        emscripten_fetch_attr_t attr;
        emscripten_fetch_attr_init(&attr);

        strcpy(attr.requestMethod, "GET");

        attr.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY;

        attr.onsuccess = DownloadSucceeded;
        attr.onerror = DownloadFailed;
        emscripten_fetch(&attr, url.c_str());
    }

    // Callbacks
    void OnSuceess(emscripten_fetch_t *fetch)
    {
        std::string payload(fetch->data, fetch->numBytes);
        emscripten_fetch_close(fetch);

        onSuccess(parsePayload(std::move(payload)));
    }

    void OnError(emscripten_fetch_t *fetch)
    {
        std::string error(fetch->url);

        onError(std::move(error), fetch->status);
        emscripten_fetch_close(fetch);
    }

    // To be overriden
    virtual void onSuccess(std::vector<MikiEntry>&& results) = 0;
    virtual void onError(std::string&&, unsigned short status) = 0;

private:
    std::vector<MikiEntry> parsePayload(std::string&& payload)
    {
        std::cout << "Payload to work with: " << payload << std::endl;

        std::vector<MikiEntry> result;
        while(!payload.empty())
        {
            std::string::size_type entryEnd = payload.find_first_of(";");
            std::string thisEntry = payload.substr(0, entryEnd);
            std::cout << "Working with: " << thisEntry << std::endl;

            payload.erase(0, entryEnd + 1);
            std::cout << "Payload left: " << payload << std::endl;

            MikiEntry mikiEntry;
            std::string::size_type delimiter = thisEntry.find_first_of("|");
            if (delimiter == std::string::npos)
            {
                std::cerr << "Something wrong" << std::endl;
                break;
            }
            mikiEntry.name = thisEntry.substr(0, delimiter);

            std::string::size_type nextDel = thisEntry.find_first_of("|", delimiter + 1);
            if (nextDel == std::string::npos)
            {
                std::cerr << "Something wrong 2 " << std::endl;
                break;
            }
            mikiEntry.path = thisEntry.substr(delimiter, nextDel - delimiter);
            mikiEntry.type = thisEntry[nextDel + 1];

            std::cout << "Got: " << mikiEntry.name << "; " << mikiEntry.path << "; " << mikiEntry.type << std::endl;

            result.push_back(mikiEntry);
        }

        return result;
    }
};

class AccessorImpl: public wrapper<Accessor>
{

public:
    EMSCRIPTEN_WRAPPER(AccessorImpl);
    // Overrides
    void onSuccess(std::vector<MikiEntry>&& elements) override {
        return call<void>("onSuccess", elements);
    }

    void onError(std::string&& error, unsigned short status) override {
        return call<void>("onError", error, status);
    }
};

EMSCRIPTEN_BINDINGS(accessor) {
    value_object<MikiEntry>("MikiEntry")
        .field("name", &MikiEntry::name)
        .field("path", &MikiEntry::path)
        .field("type", &MikiEntry::type);
    class_<Accessor>("Accessor")
        .function("onSuccess", &Accessor::onSuccess, pure_virtual())
        .function("onError", &Accessor::onError, pure_virtual())
        .function("get", &Accessor::get, allow_raw_pointers())
        .allow_subclass<AccessorImpl>("AccessorImpl");

    register_vector<MikiEntry>("vector<MikiEntry>");
}
