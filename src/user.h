#ifndef _USER_
#define _USER_

#include "includes.h"
#include "repo.h"
#include "curl-tools.h"
#include "json-tools.h"
#include "draw.h"
#include "controls.h"

class User {
private:
    std::string name;
    int numRepos;
    std::vector<Repo> repos;
    bool complete;
public:
    User(std::string name);
    void setName(std::string name);
    std::string getName();
    void setNumRepos(int numRepos);
    int getNumRepos();
    void setRepos();
    void drawListView(int x, int y);
    void drawDetailsView();
};

extern void draw_user_list(std::vector<User> user_list, int *status);

#endif
