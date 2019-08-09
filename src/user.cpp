#include "user.h"

User::User(std::string name, std::string avatar_url) {
    setName(name);
    complete = false;
    if(!avatar_url.empty()) {
        this->avatar_url = avatar_url;
        this->avatar_url += "&s=100";
    }
    else {
        this->avatar_url = "https://github.com/";
        this->avatar_url += name;
        this->avatar_url += ".png";
    }
    pfp = NULL;
    //getPfp();
    numStarred = 0;
    numRepos = 0;
}

void User::setName(std::string name) {
    this->name = name;
}

std::string User::getName() {
    return this->name;
}

void User::setNumRepos(int numRepos) {
    this->numRepos = numRepos;
}

int User::getNumRepos() {
    return this->numRepos;
}

void User::setRepos() {
    std::string url = "https://api.github.com/users/";
    url += name;

    int count = jansson_get_repo_count(curl_get_string(url));
    setNumRepos(count);

    url += "/repos?per_page=30";

    std::string json_repo_list_string = curl_get_string(url);
    jannson_get_rate_limits(curl_get_string("https://api.github.com/rate_limit"), &core_max, &core_remain, &search_max, &search_remain);

    check_core_rate_limit();

    jansson_parse_repo_list(json_repo_list_string, &repos_row0, &repos_row1);

    complete = true;
}

void User::drawListView(int y, bool selected) {
    int name_x = 40;
    if(pfp != NULL) {
        int width = vita2d_texture_get_width(pfp);
        float scale = 64.00/(float)width;
        name_x += width*scale + 10;
        draw_list_item(name, "", "", name_x, y, selected);
        vita2d_draw_texture_scale(pfp, 10, y + 50 - 32, scale, scale);
    }
    else draw_list_item(name, "", "", name_x, y, selected);
}

void User::drawDetailsView() {
    if(!complete) {
        setRepos();
    }

    int cursor_pos = 0;
    int cursor_row = 0;
    int y_offset = 44;

    int list_size0 = static_cast<int>(repos_row0.size());
    int list_size1 = static_cast<int>(repos_row1.size());

    float menuBarH = pow(500,2)/(list_size0*140);

    std::string header_string = "Following/";
    header_string += name;

    init_keys();

    int page = 1;

    while(1) {
        update_keys();
        if (down_released) {
            if(cursor_pos < list_size0 - 1) cursor_pos += 1;
            else if(list_size0 + list_size1 < numRepos && cursor_pos < list_size0) cursor_pos += 1;
            else cursor_pos = 0;
        }

        if (up_released) {
            if(cursor_pos > 0) cursor_pos -= 1;
            else if(list_size0 + list_size1 < numRepos) cursor_pos = list_size0;
            else cursor_pos = list_size0 - 1;
        }

        if(cross_released) {
            if(numRepos > 0) {
                if(cursor_pos == list_size0) {
                    page += 1;
                    //https://api.github.com/users/rinnegatamante/repos?page=2&per_page=100
                    std::string url = "https://api.github.com/users/";
                    url += name;
                    url += "/repos?page=";
                    url += std::to_string(page);
                    url += "&per_page=30";

                    jansson_parse_repo_list(curl_get_string(url), &repos_row0, &repos_row1);

                    jannson_get_rate_limits(curl_get_string("https://api.github.com/rate_limit"), &core_max, &core_remain, &search_max, &search_remain);

                    check_core_rate_limit();
                }
                else {
                    if(cursor_row == 0)
                        repos_row0[cursor_pos].drawDetailsView(header_string);
                    else if(cursor_row == 1)
                        repos_row1[cursor_pos].drawDetailsView(header_string);
                }
                list_size0 = static_cast<int>(repos_row0.size());
                list_size1 = static_cast<int>(repos_row1.size());
                menuBarH = pow(500,2)/(list_size0*140);
            }
        }

        if(left_released) {
            cursor_row -= 1;
        }

        if(right_released) {
            cursor_row += 1;
        }

        if(cursor_row < 0) {
            cursor_row = 1;
        }
        if(cursor_row > 1 || list_size1 < 1 || cursor_pos > list_size1 - 1) {
            cursor_row = 0;
        }

        if (circle_released){
            break;
        }

        while(y_offset + (cursor_pos*140) > 544 - 140) {
            y_offset -= 140;
        }

        while(y_offset + (cursor_pos*140) < 44) {
            y_offset += 140;
        }

        vita2d_start_drawing();
        vita2d_clear_screen();

        for(int i = 0; i < list_size0; i++) {
            repos_row0[i].drawListView(40, y_offset + i*140, cursor_pos == i && cursor_row == 0);
        }

        for(int i = 0; i < list_size1; i++) {
            repos_row1[i].drawListView(500, y_offset + i*140, cursor_pos == i && cursor_row == 1);
            //draw_button(std::to_string(list_size1) + std::to_string(i), 500, y_offset + i*140, 420, 140, true);
        }

        if(numRepos > list_size0 + list_size1) {
            draw_button("Load more", 40, y_offset + list_size0*140 + 40, 880, 50, cursor_pos == list_size0);
        }

        vita2d_draw_rectangle(960 - 15, 44 - y_offset*(menuBarH/500), 15, menuBarH, RGBA8(167,167,167,255));

        draw_header(header_string);

        vita2d_end_drawing();
        vita2d_common_dialog_update();
        vita2d_swap_buffers();
    }
}

void User::drawStarred() {
    if(!starred_complete) {
        setStarred(1);
    }

    int cursor_pos = 0;
    int cursor_row = 0;
    int y_offset = 44;

    int list_size0 = static_cast<int>(starred_row0.size());
    int list_size1 = static_cast<int>(starred_row1.size());

    float menuBarH = pow(500,2)/(list_size0*140);

    std::string header_string = name;
    header_string += "/Starred";

    init_keys();

    int page = 1;

    while(1) {
        update_keys();
        if (down_released) {
            if(cursor_pos < list_size0 - 1) cursor_pos += 1;
            else if(list_size0 + list_size1 < numStarred + 1 && cursor_pos < list_size0) cursor_pos += 1;
            else cursor_pos = 0;
        }

        if (up_released) {
            if(cursor_pos > 0) cursor_pos -= 1;
            else if(list_size0 + list_size1 < numStarred + 1) cursor_pos = list_size0;
            else cursor_pos = list_size0 - 1;
        }

        if(cross_released) {
            if(numStarred > 0) {
                if(cursor_pos == list_size0) {
                    while(!starred_row0.empty())
                        starred_row0.pop_back();
                    while(!starred_row1.empty())
                        starred_row1.pop_back();
                    if(list_size0 + list_size1 >= 30)
                        page += 1;
                    setStarred(page);
                }
                else {
                    if(cursor_row == 0)
                        starred_row0[cursor_pos].drawDetailsView(header_string);
                    else if(cursor_row == 1)
                        starred_row1[cursor_pos].drawDetailsView(header_string);
                }
                list_size0 = static_cast<int>(starred_row0.size());
                list_size1 = static_cast<int>(starred_row1.size());
                menuBarH = pow(500,2)/(list_size0*140);
            }
        }

        if(left_released) {
            cursor_row -= 1;
        }

        if(right_released) {
            cursor_row += 1;
        }

        if(cursor_row < 0) {
            cursor_row = 1;
        }
        if(cursor_row > 1 || list_size1 < 1 || cursor_pos > list_size1 - 1) {
            cursor_row = 0;
        }

        if (circle_released){
            break;
        }

        while(y_offset + (cursor_pos*140) > 544 - 140) {
            y_offset -= 140;
        }

        while(y_offset + (cursor_pos*140) < 44) {
            y_offset += 140;
        }

        vita2d_start_drawing();
        vita2d_clear_screen();

        for(int i = 0; i < list_size0; i++) {
            starred_row0[i].drawListView(40, y_offset + i*140, cursor_pos == i && cursor_row == 0);
        }

        for(int i = 0; i < list_size1; i++) {
            starred_row1[i].drawListView(500, y_offset + i*140, cursor_pos == i && cursor_row == 1);
            //draw_button(std::to_string(list_size1) + std::to_string(i), 500, y_offset + i*140, 420, 140, true);
        }

        if(numStarred + 1 > list_size0 + list_size1) {
            draw_button("Load more", 40, y_offset + list_size0*140 + 40, 880, 50, cursor_pos == list_size0);
        }

        vita2d_draw_rectangle(960 - 15, 44 - y_offset*(menuBarH/500), 15, menuBarH, RGBA8(167,167,167,255));

        draw_header(header_string);

        vita2d_end_drawing();
        vita2d_common_dialog_update();
        vita2d_swap_buffers();
    }
}

void User::setStarred(int page) {
    starred_complete = true;
    std::string url = "https://api.github.com/users/";
    url += name;
    url += "/starred?page=";
    url += std::to_string(page);
    url += "&per_page=30";

    numStarred += jansson_parse_repo_list(curl_get_string(url), &starred_row0, &starred_row1);

    jannson_get_rate_limits(curl_get_string("https://api.github.com/rate_limit"), &core_max, &core_remain, &search_max, &search_remain);

    check_core_rate_limit();
}

void User::getPfp() {
    curl_download_file_no_alert(avatar_url, "ux0:data/RepoHub/Downloads/pfp");
    pfp = vita2d_load_JPEG_file("ux0:data/RepoHub/Downloads/pfp");
    if(pfp == NULL)
        pfp = vita2d_load_PNG_file("ux0:data/RepoHub/Downloads/pfp");
    if(pfp == NULL)
        pfp = vita2d_load_BMP_file("ux0:data/RepoHub/Downloads/pfp");
    sceIoRemove("ux0:data/RepoHub/Downloads/pfp");
}

void User::cleanUp() {
    vita2d_free_texture(pfp);
}

//end class functions

void set_user_list(std::vector<User> *user_list, int page, bool clear) {
    while(!user_list->empty() && clear) {
        user_list->pop_back();
    }

    std::vector<std::string> userNames;
    std::vector<std::string> avatar_urls;

    std::string url = "https://api.github.com/users/";
    url += user_name;
    url += "/following?page=";
    url += std::to_string(page);
    url += "&per_page=30";

    jansson_parse_followers_list(curl_get_string(url), &userNames, &avatar_urls);
    jannson_get_rate_limits(curl_get_string("https://api.github.com/rate_limit"), &core_max, &core_remain, &search_max, &search_remain);

    for(size_t i = 0; i < userNames.size(); i++) {
        User newUser(userNames[i], avatar_urls[i]);
        user_list->push_back(newUser);
    }
}


void draw_user_list(std::vector<User> *user_list, int *status) {
    int list_size = static_cast<int>(user_list->size());
    int cursor_pos = 0;
    int y_offset = 44;
    float menuBarH = 0;
    if(list_size < following_count)
        menuBarH = pow(500,2)/((list_size+1)*95);
    else
        menuBarH = pow(500,2)/(list_size*95);
    bool done = false;
    int page = 1;

    //Rectangle posRect = {screenWidth - 10, 0 - listPos*(menuBarH/screenHeight), 5, menuBarH};

    init_keys();

    while(!done) {
        update_keys();

        if (start_released){
            //*status = QUIT_APP;
            //break;
            int state = -1;
            while(state != MAIN_VIEW) {
                state = draw_star_menu();

                if(state == UPDATE_USERNAME) {
                    set_user_name();
                    set_user_list(user_list, 1, true);
                    list_size = static_cast<int>(user_list->size());
                }
                if(state == UPDATE_TOKEN)
                    set_token();
                if(state == QUIT_APP) {
                    *status = QUIT_APP;
                    done = true;
                    state = MAIN_VIEW;
                }

                for(int i = 0; i < 3; i++) {
                    vita2d_start_drawing();
                    vita2d_clear_screen();

                    for(int i = 0; i < list_size; i++) {
                        user_list->at(i).drawListView(y_offset + i*100, cursor_pos == i);
                    }
                    vita2d_draw_rectangle(960 - 15, 44 - y_offset*(menuBarH/500), 15, menuBarH, RGBA8(167,167,167,255));

                    draw_header("Following");

                    vita2d_end_drawing();
                    vita2d_common_dialog_update();
                    vita2d_swap_buffers();
                }
            }
        }

        if (down_released) {
            if(cursor_pos < list_size - 1) cursor_pos += 1;
            else if(list_size < following_count && cursor_pos < list_size) cursor_pos += 1;
            else cursor_pos = 0;
        }

        if (up_released) {
            if(cursor_pos > 0) cursor_pos -= 1;
            else if(list_size < following_count) cursor_pos = list_size;
            else cursor_pos = list_size - 1;
        }

        if(cross_released) {
            if(list_size > 0) {
                if(cursor_pos == list_size) {
                    //if on "load more button get more users and adjust list size and menu bar size"
                    page += 1;
                    set_user_list(user_list, page, false);
                    list_size = static_cast<int>(user_list->size());
                    if(list_size <= following_count)
                        menuBarH = pow(500,2)/((list_size+1)*95);
                    else
                        menuBarH = pow(500,2)/(list_size*95);
                }
                else
                    user_list->at(cursor_pos).drawDetailsView();
            }
        }

        if(triangle_released) {
            if(list_size > 0) {
                if(cursor_pos != list_size)
                    user_list->at(0).drawStarred();
            }
        }

        if(select_released) {
            std::string title = vita_keyboard_get((char*)"Enter title", (const char*)"", 600, 0);
            std::string body = vita_keyboard_get((char*)"Enter body", (const char*)"", 600, 1);

            curl_post_issue("https://api.github.com/repos/robDevs/RepoHub/issues", title, body);
        }

        while(y_offset + (cursor_pos*100) > 544 - 100) {
            y_offset -= 100;
        }

        while(y_offset + (cursor_pos*100) < 44) {
            y_offset += 100;
        }

        vita2d_start_drawing();
        vita2d_clear_screen();

        for(int i = 0; i < list_size; i++) {
            user_list->at(i).drawListView(y_offset + i*100, cursor_pos == i);
        }
        //draw the load more button
        if(list_size < following_count)
            draw_button("Load more", 0, y_offset + list_size*100, 960, 100, cursor_pos == list_size);

        //draw the menu bar
        vita2d_draw_rectangle(960 - 15, 44 - y_offset*(menuBarH/500), 15, menuBarH, RGBA8(167,167,167,255));

        draw_header("Following");

        vita2d_end_drawing();
        vita2d_common_dialog_update();
        vita2d_swap_buffers();
    }
}
