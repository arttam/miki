#include <cstdio>
#include <iostream>
#include <string>
#include <algorithm>
#include <functional>

#include <emscripten/fetch.h>
#include <emscripten/emscripten.h>
#include <emscripten/bind.h>

using namespace emscripten;

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

        onSuccess(std::move(payload));
    }

    void OnError(emscripten_fetch_t *fetch)
    {
        std::string error(fetch->url);

        onError(std::move(error), fetch->status);
        emscripten_fetch_close(fetch);
    }

    // To be overriden
    virtual void onSuccess(std::string&& payload) = 0;
    virtual void onError(std::string&&, unsigned short status) = 0;
};

class AccessorImpl: public wrapper<Accessor>
{

public:
    EMSCRIPTEN_WRAPPER(AccessorImpl);
    // Overrides
    void onSuccess(std::string&& payload) override {
        return call<void>("onSuccess", payload);
    }

    void onError(std::string&& error, unsigned short status) override {
        return call<void>("onError", error, status);
    }
};

EMSCRIPTEN_BINDINGS(accessor) {
    class_<Accessor>("Accessor")
        .function("onSuccess", &Accessor::onSuccess, pure_virtual())
        .function("onError", &Accessor::onError, pure_virtual())
        .function("get", &Accessor::get, allow_raw_pointers())
        .allow_subclass<AccessorImpl>("AccessorImpl");
}
