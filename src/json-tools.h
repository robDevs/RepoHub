#ifndef _JSON_TOOLS_
#define _JSON_TOOLS_

#include "includes.h"
#include "repo.h"

extern int jansson_get_repo_count(std::string json_user_string);
extern int jansson_parse_repo_list(std::string json_repo_list_string, std::vector<Repo> *repoList);
extern std::string jansson_get_avatar_url(std::string json_user_string);

#endif
