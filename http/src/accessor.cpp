#include <emscripten/bind.h>
#include <emscripten/emscripten.h>
#include <emscripten/fetch.h>

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <functional>
#include <iostream>
#include <string>

using namespace emscripten;

enum class ResourceType { Folder, Page, Edit };

struct MikiEntry {
	std::string  name;
	std::string  path;
	ResourceType type;
};

// Implementation pointers
std::function<void(emscripten_fetch_t *fetch)> folderSuccessImpl;
std::function<void(emscripten_fetch_t *fetch)> pageSuccessImpl;
std::function<void(emscripten_fetch_t *fetch)> editSuccessImpl;
std::function<void(emscripten_fetch_t *fetch)> downloadFailedImpl;
std::function<void(emscripten_fetch_t *fetch)> postSuccessImpl;
std::function<void(emscripten_fetch_t *fetch)> postFailedImpl;

// Declaration pointers
void FolderSuccess(emscripten_fetch_t *fetch)
{
	folderSuccessImpl(fetch);
}

void PageSuccess(emscripten_fetch_t *fetch)
{
	pageSuccessImpl(fetch);
}

void EditSuccess(emscripten_fetch_t *fetch)
{
	editSuccessImpl(fetch);
}

void DownloadFailed(emscripten_fetch_t *fetch)
{
	downloadFailedImpl(fetch);
}

void PostSuccess(emscripten_fetch_t *fetch)
{
	postSuccessImpl(fetch);
}

void PostFailed(emscripten_fetch_t *fetch)
{
	postFailedImpl(fetch);
}

class Accessor {
public:
	Accessor()
	{
		folderSuccessImpl  = std::bind(&Accessor::OnFolderSuccess, this, std::placeholders::_1);
		pageSuccessImpl    = std::bind(&Accessor::OnPageSuccess, this, std::placeholders::_1);
		editSuccessImpl    = std::bind(&Accessor::OnEditSuccess, this, std::placeholders::_1);
		downloadFailedImpl = std::bind(&Accessor::OnError, this, std::placeholders::_1);
		postSuccessImpl    = std::bind(&Accessor::OnPostSuccess, this, std::placeholders::_1);
		postFailedImpl     = std::bind(&Accessor::OnPostError, this, std::placeholders::_1);
	}
	virtual ~Accessor() {}

	// Request
	void get(const std::string url, ResourceType resourceType)
	{
		emscripten_fetch_attr_t attr;
		emscripten_fetch_attr_init(&attr);

		strcpy(attr.requestMethod, "GET");

		attr.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY;

		switch (resourceType) {
			case ResourceType::Folder:
				attr.onsuccess = FolderSuccess;
				break;
			case ResourceType::Page:
				attr.onsuccess = PageSuccess;
				break;
			case ResourceType::Edit:
				attr.onsuccess = EditSuccess;
				break;
		}

		// attr.onsuccess = FolderSuccess;
		attr.onerror = DownloadFailed;
		emscripten_fetch(&attr, url.c_str());
	}

	// Post
	void post(const std::string url, std::string &&payload)
	{
		emscripten_fetch_attr_t attr;
		emscripten_fetch_attr_init(&attr);

		strcpy(attr.requestMethod, "POST");

		attr.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY;
		attr.onsuccess  = PostSuccess;
		attr.onerror    = PostFailed;

		const char *headers[] = {"Content-Type", "application/text", 0};
		attr.requestHeaders   = headers;
		attr.requestData      = strdup(payload.c_str());
		attr.requestDataSize  = payload.length();

		emscripten_fetch(&attr, url.c_str());
	}

	// Callbacks
	void OnFolderSuccess(emscripten_fetch_t *fetch)
	{
		std::string payload(fetch->data, fetch->numBytes);
		emscripten_fetch_close(fetch);

		onFolder(parsePayload(std::move(payload)));
	}

	void OnPageSuccess(emscripten_fetch_t *fetch)
	{
		std::string payload(fetch->data, fetch->numBytes);
		emscripten_fetch_close(fetch);

		onPage(std::move(payload));
	}

	void OnEditSuccess(emscripten_fetch_t *fetch)
	{
		std::string payload(fetch->data, fetch->numBytes);
		emscripten_fetch_close(fetch);

		onEdit(std::move(payload));
	}

	void OnError(emscripten_fetch_t *fetch)
	{
		std::string error(fetch->url);

		onError(std::move(error), fetch->status);
		emscripten_fetch_close(fetch);
	}

	void OnPostSuccess(emscripten_fetch_t *fetch)
	{
		std::string payload(fetch->data, fetch->numBytes);
		emscripten_fetch_close(fetch);

		onPostSuccess(std::move(payload));
	}

	void OnPostError(emscripten_fetch_t *fetch)
	{
		std::string error(fetch->data, fetch->numBytes);
		emscripten_fetch_close(fetch);

		onPostError(std::move(error), fetch->status);
	}

	// To be overriden
	virtual void onFolder(std::vector<MikiEntry> &&results)        = 0;
	virtual void onPage(std::string &&page)                        = 0;
	virtual void onEdit(std::string &&page)                        = 0;
	virtual void onError(std::string &&, unsigned short status)    = 0;
	virtual void onPostSuccess(std::string &&response)             = 0;
	virtual void onPostError(std::string &&, unsigned short staus) = 0;

private:
	std::vector<MikiEntry> parsePayload(std::string &&payload)
	{
		std::vector<MikiEntry> result;
		while (!payload.empty()) {
			std::string::size_type entryEnd  = payload.find_first_of(";");
			std::string            thisEntry = payload.substr(0, entryEnd);

			payload.erase(0, entryEnd + 1);

			MikiEntry              mikiEntry;
			std::string::size_type delimiter = thisEntry.find_first_of("|");
			if (delimiter == std::string::npos) {
				std::cerr << "Something wrong - cannot find delimiter '|' in " << thisEntry << std::endl;
				break;
			}
			mikiEntry.name = thisEntry.substr(0, delimiter);

			std::string::size_type nextDel = thisEntry.find_first_of("|", ++delimiter);
			if (nextDel == std::string::npos) {
				std::cerr << "Something wrong - cannot find second delimiter in " << thisEntry << std::endl;
				break;
			}
			mikiEntry.path = thisEntry.substr(delimiter, nextDel - delimiter);
			switch (thisEntry[nextDel + 1]) {
				case 'p':
					mikiEntry.type = ResourceType::Page;
					break;
				case 'd':
					mikiEntry.type = ResourceType::Folder;
					break;
			}

			result.push_back(mikiEntry);
		}

		return result;
	}
};

class AccessorImpl : public wrapper<Accessor> {
public:
	EMSCRIPTEN_WRAPPER(AccessorImpl);
	// Overrides
	void onFolder(std::vector<MikiEntry> &&elements) override
	{
		return call<void>("onFolder", elements);
	}

	void onPage(std::string &&page) override
	{
		return call<void>("onPage", page);
	}

	void onEdit(std::string &&page) override
	{
		return call<void>("onEdit", page);
	}

	void onError(std::string &&error, unsigned short status) override
	{
		return call<void>("onError", error, status);
	}

	void onPostSuccess(std::string &&response) override
	{
		return call<void>("onPostSuccess", response);
	}

	void onPostError(std::string &&errorText, unsigned short errorCode) override
	{
		return call<void>("onPostError", errorText, errorCode);
	}
};

// clang-format off
EMSCRIPTEN_BINDINGS(accessor)
{
	enum_<ResourceType>("ResourceType")
        .value("Folder", ResourceType::Folder)
        .value("Page", ResourceType::Page)
        .value("Edit", ResourceType::Edit);
	value_object<MikiEntry>("MikiEntry")
        .field("name", &MikiEntry::name)
        .field("path", &MikiEntry::path)
        .field("type", &MikiEntry::type);
	class_<Accessor>("Accessor")
	    .function("onFolder", &Accessor::onFolder, pure_virtual())
	    .function("onPage", &Accessor::onPage, pure_virtual())
	    .function("onEdit", &Accessor::onEdit, pure_virtual())
	    .function("onError", &Accessor::onError, pure_virtual())
	    .function("onPostSuccess", &Accessor::onPostSuccess, pure_virtual())
	    .function("onPostError", &Accessor::onPostError, pure_virtual())
	    .function("get", &Accessor::get, allow_raw_pointers())
	    .function("post", &Accessor::post, allow_raw_pointers())
	    .allow_subclass<AccessorImpl>("AccessorImpl");

	register_vector<MikiEntry>("vector<MikiEntry>");
}
// clang-format on
