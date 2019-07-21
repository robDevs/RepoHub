#ifndef _USER_
#define _USER_

#include "includes.h"
#include "repo.h"
#include "curl-tools.h"
#include "json-tools.h"
#include "draw.h"
#include "controls.h"
#include "vita-keyboard.h"
#include <psp2/appmgr.h>


class User {
private:
    std::string name;
    int numRepos, repo_list0_size, repo_list1_size;
    std::vector<Repo> repos_row0, repos_row1;
    bool complete;
public:
    User(std::string name);
    void setName(std::string name);
    std::string getName();
    void setNumRepos(int numRepos);
    int getNumRepos();
    void setRepos();
    void drawListView(int y, bool selected);
    void drawDetailsView();
    void drawReleases();
};

extern void draw_user_list(std::vector<User> user_list, int *status);

#endif
