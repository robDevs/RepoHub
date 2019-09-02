#ifndef _JSON_TOOLS_
#define _JSON_TOOLS_

#include "includes.h"
#include "repo.h"
#include "release.h"
#include "user.h"

extern void jansson_parse_user_info(std::string json_user_string, User *user);
extern int jansson_get_repo_count(std::string json_user_string);
extern int jansson_parse_repo_list(std::string json_repo_list_string, std::vector<Repo> *repoList0, std::vector<Repo> *repoList1);
extern std::string jansson_get_avatar_url(std::string json_user_string);

extern int jansson_parse_release_list(std::string json_release_list_string, std::vector<Release> *releaseList);
extern void jansson_parse_asset_list(json_t *json_array, std::vector<Asset> *assetList);

extern void jannson_get_rate_limits(std::string limit_string, int *core_max, int *core_min, int *search_max, int *search_min);

extern void jansson_parse_followers_list(std::string followers_list, std::vector<std::string> *list, std::vector<std::string> *list0);
extern int jannson_get_following_count(std::string json_user_string);

extern void jansson_get_readme(std::string readme_string, std::vector<std::string> *readme_vec);

extern void jansson_get_authed_user(std::string user_string, std::string *user_name, bool *authed, int *following_count);

//used only for getting the tag for RepoHub latest release. To be compared with #define VERSION  x.xx
extern void jansson_get_tag_from_release(std::string release_string, float *tag, std::string *url);

extern void jansson_parse_user_search(std::string results, std::vector<std::string> *user_list);
extern int jansson_parse_repo_search(std::string results, std::vector<Repo> *repoList0, std::vector<Repo> *repoList1);

#endif
