#include "resource.h"

// Fields separated by '|'
Resource::Resource(const std::string& src) 
    : name_{}
    , fullPath_{}
    , type_{ResourceType::Other}
{
    if (src.empty()) {
        return;
    }

    size_t elementNr{0};
    size_t partStart{0};
    size_t partEnd = src.find_first_of('|'); 
    while (partEnd != std::string::npos)
    {
        switch (elementNr)
        {
            case 0:
                name_ = src.substr(partStart, partEnd - 1);
                break;
            case 1:
                fullPath_ = src.substr(partStart, partEnd - 1);
                break;
            case 2:
                {
                    switch (src.substr(partStart, 1)[0]) {
                        case 'd':
                            type_ = ResourceType::Folder;
                            break;
                        case 'p':
                            type_ = ResourceType::Page;
                            break;
                        default:
                            type_ = ResourceType::Other;
                            break;
                    }
                }
                break;
            default:
                // something wrong should not be here at all
                partEnd = std::string::npos;
                partStart = std::string::npos;
                break;
        }

        if (partEnd != std::string::npos) {
            ++elementNr;
            partStart = partEnd + 1;
            partEnd   = src.find_first_of(partStart, '|');
        }
    }

    if (partStart != std::string::npos && elementNr == 2) {
        switch (src.substr(partStart, 1)[0]) {
            case 'd':
                type_ = ResourceType::Folder;
                break;
            case 'p':
                type_ = ResourceType::Page;
                break;
            default:
                type_ = ResourceType::Other;
                break;
        }
    }
}

Resource::Resource(const std::string& name, const std::string& path, char type)
    : name_{name}
    , fullPath_{path}
{
    switch (type) {
        case 'd':
            type_ = ResourceType::Folder;
            break;
        case 'p':
            type_ = ResourceType::Page;
            break;
        default:
            type_ = ResourceType::Other;
            break;
    }
}

std::string Resource::asString() const
{
    std::string response {};

    response.append(name_).append("|").append(fullPath_).append("|");

    switch (type_) {
        case ResourceType::Folder:
            response.append("d");
            break;
        case ResourceType::Page:
            response.append("p");
            break;
        default:
            response.append("o");
    }

    return response;
}

bool Resource::operator <(const Resource& other) const
{
    // Folder, File, Other
    if (type_ == other.type_)
    {
        return (name_ < other.name_);
    }

    if (type_ == ResourceType::Folder && other.type_ != ResourceType::Folder)
    {
        return true;
    }
    else if (other.type_ == ResourceType::Folder)
    {
        return false;
    }
    else if (type_ == ResourceType::Page && other.type_ != ResourceType::Page)
    {
        return true;
    }
    else if (other.type_ == ResourceType::Page)
    {
        return false;
    }
    else 
    {
        return (type_ != ResourceType::Other);
    }

    return false;
}

std::ostream& operator<<(std::ostream& os, const Resource& resource);


