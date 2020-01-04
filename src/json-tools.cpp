#include "json-tools.h"
#include "draw.h"

void jansson_parse_user_info(std::string json_user_string, User *user) {
    json_t *root;
    json_error_t error;

    root = json_loads(json_user_string.c_str(), 0, &error);

    if(!root)
    {
        return;
    }

    if(!json_is_object(root))
    {
        json_decref(root);
        return;
    }

    json_t *type, *name, *company, *location, *email, *bio;

    type = json_object_get(root, "type");
    name = json_object_get(root, "name");
    company = json_object_get(root, "company");
    location = json_object_get(root, "location");
    email = json_object_get(root, "email");
    bio = json_object_get(root, "bio");

    if(json_is_string(type))
        user->setType(json_string_value(type));
    if(json_is_string(name))
        user->setRname(json_string_value(name));
    if(json_is_string(company))
        user->setCompany(json_string_value(company));
    if(json_is_string(location))
        user->setLocation(json_string_value(location));
    if(json_is_string(email))
        user->setEmail(json_string_value(email));
    if(json_is_string(bio))
        user->setBio(json_string_value(bio));

    user->setNumRepos(jansson_get_repo_count(json_user_string));
    user->setAvatarUrl(jansson_get_avatar_url(json_user_string));

    json_decref(root);
    return;
}

int jansson_get_repo_count(std::string json_user_string) {
    json_t *root;
    json_error_t error;

    root = json_loads(json_user_string.c_str(), 0, &error);

    if(!root)
    {
        return 0;
    }

    if(!json_is_object(root))
    {
        json_decref(root);
        return 0;
    }

    json_t *data;

    data = json_object_get(root, "public_repos"); // we only care about public repos.

    if(!json_is_integer(data))
    {
        json_decref(root);
        return 0;
    }

    int returnVal = json_integer_value(data);
    json_decref(root);
    return returnVal;
}

int jansson_parse_repo_list(std::string json_repo_list_string, std::vector<Repo> *repoList0, std::vector<Repo> *repoList1) {
    json_t *root;
    json_error_t error;

    int count = 0;

    root = json_loads(json_repo_list_string.c_str(), 0, &error);

    if(!json_is_array(root))
    {
        json_decref(root);
        return 0;
    }

    int alt = 0;

    for(size_t i = 0; i < json_array_size(root); i++) {
        json_t *data, *name, *priv, *description, *fork, *created_at, *updated_at, *homePage, *stargazers_count, *language, *license, *forks, *open_issues_count, *owner;

        std::string nameString, descriptionString, created_atString, updated_atString, homePageString, languageString, licenseString, owner_string;
        bool privBool, forkBool;
        int stargazers_countInt, forksInt, open_issues_countInt;
        data = json_array_get(root, i);
        if(!json_is_object(data))
        {
            json_decref(root);
            return count;
        }
        name = json_object_get(data, "name");
        if(json_is_string(name)) nameString = json_string_value(name);

        priv = json_object_get(data, "private");
        if(json_is_boolean(priv)) privBool = json_boolean_value(priv);
        //do owner info here
        description = json_object_get(data, "description");
        if(json_is_string(description)) descriptionString = json_string_value(description);

        fork = json_object_get(data, "fork");
        if(json_is_boolean(fork)) forkBool = json_boolean_value(fork);

        created_at = json_object_get(data, "created_at");
        if(json_is_string(created_at)) created_atString = json_string_value(created_at);

        updated_at = json_object_get(data, "pushed_at");
        if(json_is_string(updated_at)) updated_atString = json_string_value(updated_at);

        homePage = json_object_get(data, "homePage");
        if(json_is_string(homePage)) homePageString = json_string_value(homePage);

        stargazers_count = json_object_get(data, "stargazers_count");
        if(json_is_integer(stargazers_count)) stargazers_countInt = json_integer_value(stargazers_count);

        language = json_object_get(data, "language");
        if(json_is_string(language)) languageString = json_string_value(language);

        license = json_object_get(data, "license");
        if(json_is_object(license)) {
            json_t *license_name = json_object_get(license, "name");
            if(json_is_string(license_name)) licenseString = json_string_value(license_name);
        }

        forks = json_object_get(data, "forks");
        if(json_is_integer(forks)) forksInt = json_integer_value(forks);

        open_issues_count = json_object_get(data, "open_issues_count");
        if(json_is_integer(open_issues_count)) open_issues_countInt = json_integer_value(open_issues_count);

        owner = json_object_get(data, "owner");
        if(json_is_object(owner)) {
            json_t *owner_name = json_object_get(owner, "login");
            if(json_is_string(owner_name))
                owner_string = json_string_value(owner_name);
            else owner_string = "";
        }

        Repo newRepo(nameString,
                     descriptionString,
                     licenseString,
                     owner_string,
                     created_atString,
                     updated_atString,
                     homePageString,
                     languageString,
                     forkBool,
                     privBool,
                     forksInt,
                     stargazers_countInt,
                     open_issues_countInt
                 );
        if(alt == 0)
            repoList0->push_back(newRepo);
        else if(alt == 1)
            repoList1->push_back(newRepo);

        alt += 1;
        if(alt > 1)
            alt = 0;

        count += 1;
    }

    json_decref(root);
    return count;

}

std::string jansson_get_avatar_url(std::string json_user_string) {
    json_t *root;
    json_error_t error;

    root = json_loads(json_user_string.c_str(), 0, &error);

    if(!root)
    {
        return "Error getting image url";
    }

    if(!json_is_object(root))
    {
        json_decref(root);
        return "Error getting image url";
    }

    json_t *data;

    data = json_object_get(root, "avatar_url");
    if(!json_is_string(data))
    {
        json_decref(root);
        return "Error getting image url";
    }

    std::string returnVal = json_string_value(data);
    json_decref(root);
    return returnVal;
}

int jansson_parse_release_list(std::string json_release_list_string, std::vector<Release> *releaseList) {
    json_t *root;
    json_error_t error;

    root = json_loads(json_release_list_string.c_str(), 0, &error);

    if(!root)
    {
        return 0;
    }

    if(!json_is_array(root))
    {
        json_decref(root);
        return 0;
    }

    int count = 0;

    for(size_t i = 0; i < json_array_size(root); i++) {
        json_t *data, *tag_name, *name, *published_at, *body, *asset_array, *draft, *prerelease, *author, *author_name, *src_url;
        std::vector<Asset> assets;

        data = json_array_get(root, i);
        if(!json_is_object(data))
        {
            json_decref(root);
            return count;
        }

        tag_name = json_object_get(data, "tag_name"); // we only care about public repos.
        name = json_object_get(data, "name");
        published_at = json_object_get(data, "published_at");
        body = json_object_get(data, "body");
        draft = json_object_get(data, "draft");
        prerelease = json_object_get(data, "prerelease");

        author = json_object_get(data, "author");
        if(json_is_object(author)) {
            author_name = json_object_get(author, "login");
        }

        src_url = json_object_get(data, "zipball_url");

        asset_array = json_object_get(data, "assets");
        if(json_is_array(asset_array)) {
            for(size_t c = 0; c < json_array_size(asset_array); c++) {
                json_t *asset_data, *asset_name, *asset_url, *asset_size;

                asset_data = json_array_get(asset_array, c);
                if(!json_is_object(asset_data))
                    break;
                asset_name = json_object_get(asset_data, "name");
                asset_url = json_object_get(asset_data, "browser_download_url");
                asset_size = json_object_get(asset_data, "size");

                Asset new_asset;
                if(json_is_string(asset_name))
                    new_asset.name = json_string_value(asset_name);
                if(json_is_string(asset_url))
                    new_asset.url = json_string_value(asset_url);
                if(json_is_integer(asset_size))
                    new_asset.size = json_integer_value(asset_size);
                assets.push_back(new_asset);
            }
                Asset src_asset;
                if(json_is_string(name)) {
                    src_asset.name = json_string_value(name);
                    src_asset.name += "_source.zip";
                }
                if(json_is_string(src_url))
                    src_asset.url = json_string_value(src_url);
                src_asset.size = 0;

                assets.push_back(src_asset);
        }

        Release new_release;
        if(json_is_string(tag_name))
            new_release.tag_name = json_string_value(tag_name);
        if(json_is_string(name))
            new_release.name = json_string_value(name);
        if(json_is_string(published_at))
            new_release.published_at = json_string_value(published_at);
        if(json_is_string(body))
            new_release.body = json_string_value(body);
        if(json_is_string(author_name))
            new_release.author = json_string_value(author_name);
            //stripUnicode(new_release.body);
        if(json_is_boolean(prerelease))
            new_release.prerelease = json_boolean_value(prerelease);
        for(size_t x = 0; x < assets.size(); x++)
            new_release.assets.push_back(assets[x]);

        if(json_is_boolean(draft)) {
            if(json_boolean_value(draft) == false) {
                count += 1;
                releaseList->push_back(new_release);
            }
        }
    }

    json_decref(root);
    return count;
}
void jansson_parse_asset_list(json_t *json_array, std::vector<Asset> *assetList) {
    if(!json_is_array(json_array)) {
        return;
    }

    for(size_t i = 0; i < json_array_size(json_array); i++) {
        json_t *data, *name, *url;

        data = json_array_get(json_array, i);
        if(!json_is_object(data))
            return;
        name = json_object_get(data, "name");
        url = json_object_get(data, "browser_download_url");

        Asset new_asset;
        if(json_is_string(name))
            new_asset.name = json_string_value(name);
        if(json_is_string(url))
            new_asset.url = json_string_value(url);
        assetList->push_back(new_asset);
    }
}

void jannson_get_rate_limits(std::string limit_string, int *core_max, int *core_min, int *search_max, int *search_min) {
    json_t *root, *json_core, *json_search, *data, *json_core_max, *json_core_min, *json_search_max, *json_search_min;
    json_error_t error;

    root = json_loads(limit_string.c_str(), 0, &error);

    if(!root)
    {
        core_max = 0;
        core_min = 0;
        search_max = 0;
        search_min = 0;
        return;
    }

    if(!json_is_object(root))
    {
        core_max = 0;
        core_min = 0;
        search_max = 0;
        search_min = 0;
        json_decref(root);
        return;
    }

    data = json_object_get(root, "resources");

    if(!json_is_object(data)) {
        core_max = 0;
        core_min = 0;
        search_max = 0;
        search_min = 0;
        json_decref(root);
        return;
    }


    json_core = json_object_get(data, "core");
    json_core_max = json_object_get(json_core, "limit");
    json_core_min = json_object_get(json_core, "remaining");

    json_search = json_object_get(data, "search");
    json_search_max = json_object_get(json_search, "limit");
    json_search_min = json_object_get(json_search, "remaining");

    if(json_is_integer(json_core_max))
        *core_max = json_integer_value(json_core_max);
    if(json_is_integer(json_core_min))
        *core_min = json_integer_value(json_core_min);
    if(json_is_integer(json_search_max))
        *search_max = json_integer_value(json_search_min);
    if(json_is_integer(json_search_min))
        *search_min = json_integer_value(json_search_min);

    json_decref(root);
    return;

}

void jansson_parse_followers_list(std::string followers_list, std::vector<std::string> *list, std::vector<std::string> *list0) {
    json_t *root;
    json_error_t error;

    root = json_loads(followers_list.c_str(), 0, &error);

    if(!root)
    {
        return;
    }

    if(!json_is_array(root))
    {
        json_decref(root);
        return;
    }

    for(size_t i = 0; i < json_array_size(root); i++) {
        json_t *data, *user_name, *avatar_url;

        data = json_array_get(root, i);
        if(!json_is_object(data))
        {
            json_decref(root);
            return;
        }

        user_name = json_object_get(data, "login");
        avatar_url = json_object_get(data, "avatar_url");


        if(json_is_string(user_name) && json_is_string(avatar_url)) {
            list->push_back(json_string_value(user_name));
            list0->push_back(json_string_value(avatar_url));
        }
    }
    json_decref(root);
}

int jannson_get_following_count(std::string json_user_string) {
    json_t *root;
    json_error_t error;

    root = json_loads(json_user_string.c_str(), 0, &error);

    if(!root)
    {
        return 0;
    }

    if(!json_is_object(root))
    {
        json_decref(root);
        return 0;
    }

    json_t *data;

    data = json_object_get(root, "following"); // we only care about public repos.

    if(!json_is_integer(data))
    {
        json_decref(root);
        return 0;
    }

    int returnVal = json_integer_value(data);
    json_decref(root);
    return returnVal;
}

void jansson_get_readme(std::string readme_string, std::vector<std::string> *readme_vec) {
    json_t *root;
    json_error_t error;

    root = json_loads(readme_string.c_str(), 0, &error);

    if(!root)
    {
        return;
    }

    if(!json_is_object(root))
    {
        json_decref(root);
        return;
    }

    json_t *data = json_object_get(root, "download_url");

    if(json_is_string(data)) {
        curl_download_file_no_alert(json_string_value(data), "ux0:data/RepoHub/Downloads/temp.txt");
        json_decref(root);
        read_file_lines("ux0:data/RepoHub/Downloads/temp.txt", readme_vec);
        sceIoRemove("ux0:data/RepoHub/Downloads/temp.txt");
    }
}

void jansson_get_authed_user(std::string user_string, std::string *user_name, bool *authed, int *following_count) {
    json_t *root, *message, *json_user_name, *json_following_count;
    json_error_t error;

    root = json_loads(user_string.c_str(), 0, &error);

    if(!root)
    {
        return;
    }

    if(!json_is_object(root)) {
        json_decref(root);
        *authed = false;
        return;
    }

    message = json_object_get(root, "message");
    if(json_is_string(message)) {
        *authed = false;
        json_decref(root);
        return;
    }

    json_user_name = json_object_get(root, "login");
    json_following_count = json_object_get(root, "following");

    if(json_is_string(json_user_name))
        *user_name = json_string_value(json_user_name);
    if(json_is_integer(json_following_count))
        *following_count = json_integer_value(json_following_count);

    *authed = true;
    json_decref(root);
    return;
}

void jansson_get_tag_from_release(std::string release_string, float *tag, std::string *url) {
    json_t *root;
    json_error_t error;

    root = json_loads(release_string.c_str(), 0, &error);

    if(!root)
    {
        *tag = 0.00;
        return;
    }

    if(!json_is_object(root))
    {
        json_decref(root);
        *tag = 0.00;
        return;
    }

    json_t *json_tag, *json_url;

    json_tag = json_object_get(root, "tag_name");
    json_url = json_object_get(json_array_get(json_object_get(root, "assets"), 0), "browser_download_url");

    if(!json_is_string(json_tag))
    {
        *tag = 0.00;
    }
    else
    {
        std::string string_all = json_string_value(json_tag);
        string_all.erase(string_all.begin());
        float ret_val = std::stof(string_all);

        *tag = ret_val;
    }

    if(!json_is_string(json_url)) {
        *url = "";
    }
    else
    {
        *url = json_string_value(json_url);
    }

    json_decref(root);
    return;
}

void jansson_parse_user_search(std::string results, std::vector<std::string> *user_list) {
    json_t *root;
    json_error_t error;

    root = json_loads(results.c_str(), 0, &error);

    if(!root) {
        return;
    }

    if(!json_is_object(root)) {
        json_decref(root);
        return;
    }

    json_t *items;

    items = json_object_get(root, "items");

    if(!json_is_array(items)) {
        json_decref(root);
        return;
    }

    for(size_t i = 0; i < json_array_size(items); i++) {
        json_t *data, *login, *avatar_url;

        data = json_array_get(items, i);

        if(!json_is_object(data)) {
            json_decref(root);
            return;
        }

        login = json_object_get(data, "login");
        avatar_url = json_object_get(data, "avatar_url");

        if(json_is_string(login) && json_is_string(avatar_url)) {
            //User newUser(json_string_value(login), json_string_value(avatar_url));
            user_list->push_back(json_string_value(login));
        }
    }

    json_decref(root);
    return;
}

int jansson_parse_repo_search(std::string results, std::vector<Repo> *repoList0, std::vector<Repo> *repoList1) {
    json_t *root;
    json_error_t error;

    int count = 0;
    int alt = 0;

    root = json_loads(results.c_str(), 0, &error);

    if(!root) {
        return 0;
    }

    if(!json_is_object(root)) {
        json_decref(root);
        return 0;
    }

    json_t *items;

    items = json_object_get(root, "items");

    if(!json_is_array(items)) {
        json_decref(root);
        return 0;
    }

    for(size_t i = 0; i < json_array_size(items); i++) {
        json_t *data, *name, *priv, *description, *fork, *created_at, *updated_at, *homePage, *stargazers_count, *language, *license, *forks, *open_issues_count, *owner;

        std::string nameString, descriptionString, created_atString, updated_atString, homePageString, languageString, licenseString, owner_string;
        bool privBool, forkBool;
        int stargazers_countInt, forksInt, open_issues_countInt;
        data = json_array_get(items, i);
        if(!json_is_object(data))
        {
            json_decref(root);
            return count;
        }
        name = json_object_get(data, "name");
        if(json_is_string(name)) nameString = json_string_value(name);

        priv = json_object_get(data, "private");
        if(json_is_boolean(priv)) privBool = json_boolean_value(priv);
        //do owner info here
        description = json_object_get(data, "description");
        if(json_is_string(description)) descriptionString = json_string_value(description);

        fork = json_object_get(data, "fork");
        if(json_is_boolean(fork)) forkBool = json_boolean_value(fork);

        created_at = json_object_get(data, "created_at");
        if(json_is_string(created_at)) created_atString = json_string_value(created_at);

        updated_at = json_object_get(data, "pushed_at");
        if(json_is_string(updated_at)) updated_atString = json_string_value(updated_at);

        homePage = json_object_get(data, "homePage");
        if(json_is_string(homePage)) homePageString = json_string_value(homePage);

        stargazers_count = json_object_get(data, "stargazers_count");
        if(json_is_integer(stargazers_count)) stargazers_countInt = json_integer_value(stargazers_count);

        language = json_object_get(data, "language");
        if(json_is_string(language)) languageString = json_string_value(language);

        license = json_object_get(data, "license");
        if(json_is_object(license)) {
            json_t *license_name = json_object_get(license, "name");
            if(json_is_string(license_name)) licenseString = json_string_value(license_name);
        }

        forks = json_object_get(data, "forks");
        if(json_is_integer(forks)) forksInt = json_integer_value(forks);

        open_issues_count = json_object_get(data, "open_issues_count");
        if(json_is_integer(open_issues_count)) open_issues_countInt = json_integer_value(open_issues_count);

        owner = json_object_get(data, "owner");
        if(json_is_object(owner)) {
            json_t *owner_name = json_object_get(owner, "login");
            if(json_is_string(owner_name))
                owner_string = json_string_value(owner_name);
            else owner_string = "";
        }

        Repo newRepo(nameString,
                     descriptionString,
                     licenseString,
                     owner_string,
                     created_atString,
                     updated_atString,
                     homePageString,
                     languageString,
                     forkBool,
                     privBool,
                     forksInt,
                     stargazers_countInt,
                     open_issues_countInt
                 );
        if(alt == 0)
            repoList0->push_back(newRepo);
        else if(alt == 1)
            repoList1->push_back(newRepo);

        alt += 1;
        if(alt > 1)
            alt = 0;

        count += 1;
    }

    json_decref(root);
    return count;
}

