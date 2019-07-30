#ifndef _JSON_TOOLS_
#define _JSON_TOOLS_

#include "includes.h"
#include "repo.h"
#include "release.h"

extern int jansson_get_repo_count(std::string json_user_string);
extern int jansson_parse_repo_list(std::string json_repo_list_string, std::vector<Repo> *repoList0, std::vector<Repo> *repoList1);
extern std::string jansson_get_avatar_url(std::string json_user_string);

extern int jansson_parse_release_list(std::string json_release_list_string, std::vector<Release> *releaseList);
extern void jansson_parse_asset_list(json_t *json_array, std::vector<Asset> *assetList);

extern void jannson_get_rate_limits(std::string limit_string, int *core_max, int *core_min, int *search_max, int *search_min);

extern void jansson_parse_followers_list(std::string followers_list, std::vector<std::string> *list, std::vector<std::string> *list0);

extern void jansson_get_readme(std::string readme_string, std::vector<std::string> *readme_vec);

#endif
