#ifndef _REPO_
#define _REPO_

#include "includes.h"
#include "images.h"
#include "controls.h"
#include "draw.h"
#include "release.h"
#include "curl-tools.h"

class Repo {
private:
    std::string name;
    std::string description;
    std::string license;
    std::string owner;
    std::string created_at;
    std::string updated_at;
    std::string homePage;
    std::string language;
    std::string readme;
    std::vector<std::string> readme_vec;
    std::vector<Release> releases;
    bool fork;
    bool priv;
    int forks;
    int stargazers_count;
    int open_issues_count;
public:
    Repo(std::string name, std::string description, std::string license, std::string owner, std::string created_at, std::string updated_at, std::string homePage, std::string language, bool fork, bool priv, int forks, int stargazers_count, int open_issues_count);
    void setName(std::string name);
    std::string getName();
    void setOwner(std::string owner);
    void setLicense(std::string license);
    std::string getLicense();
    void setFork(bool fork);
    bool getFork();
    void setForks(int forks);
    int getForks();
    void drawListView(int x, int y, bool selected, bool showOwner=false);
    void drawDetailsView(std::string header_start);
    void drawReleases(std::string header_start);
    void setReadme();
    void draw_readme_vec(int y);
    int get_readme_vec_height();
};
#endif
