#ifndef _RELEASE_
#define _RELEASE_

#include "includes.h"

struct Asset {
    std::string name;
    std::string url;
    uint64_t size;
};

struct Release {
    std::string name;
    std::string tag_name;
    std::string body;
    std::string created_at;
    std::vector<Asset> assets;
    bool draft;
    bool prerelease;
};

extern void draw_release_details(Release release);


#endif
