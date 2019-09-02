#ifndef _USER_
#define _USER_

#include "includes.h"
#include "repo.h"
#include "curl-tools.h"
#include "draw.h"
#include "controls.h"
#include "vita-keyboard.h"


class User {
private:
    std::string name, Rname, type, company, location, email, bio;
    int numRepos, numStarred, repo_list0_size, repo_list1_size;
    std::vector<Repo> repos_row0, repos_row1;
    std::vector<Repo> starred_row0, starred_row1;
    bool haveInfo, complete;
    bool starred_complete;
    std::string avatar_url;
    vita2d_texture *pfp;
public:
    User(std::string name, std::string avatar_url);
    void setName(std::string name);
    std::string getName();
    void setNumRepos(int numRepos);
    void setRname(std::string Rname);
    void setType(std::string type);
    void setCompany(std::string company);
    void setLocation(std::string location);
    void setEmail(std::string email);
    void setBio(std::string bio);
    void setAvatarUrl(std::string url);
    int getNumRepos();
    void setRepos();
    void drawListView(int y, bool selected);
    void drawDetailsView();
    void drawReposView();
    void drawReleases();
    void getPfp();
    void cleanUp();
    void setStarred(int page);
    void drawStarred();
    void get_nums();
};

extern void set_user_list(std::vector<User> *user_list, int page, bool clear);
extern void draw_user_list(std::vector<User> *user_list, int *status);


#endif
