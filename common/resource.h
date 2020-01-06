#ifndef __USES_RESOURCE_H__
#define __USES_RESOURCE_H__

#include <iostream>
#include <string>

enum class ResourceType : char
{
    Folder = 'd', 
    Page   = 'p', 
    Other  = 'o'
};

class Resource
{
    std::string name_;
    std::string fullPath_;
    ResourceType type_;

public:
    Resource() = default;
    Resource(const Resource& src) = default;
    Resource(Resource&& obj) = default;
    Resource& operator=(const Resource& other) = default;
    Resource& operator=(Resource&& other) = default;

    Resource(const std::string& stringSrc);
    Resource(const std::string& name, const std::string& path, char type);

    ~Resource() = default;

    std::string name()     const { return name_; }
    std::string fullPath() const { return fullPath_; };
    ResourceType type()    const { return type_; };

    void name(const std::string& name) { name_ = name; }
    void fullPath(const std::string& path) { fullPath_ = path; }
    void type(char cType) {
        switch (cType) {
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

    std::string asString() const;

    bool operator <(const Resource& other) const;
};

std::ostream& operator << (std::ostream& os, const Resource& resource);

#endif
