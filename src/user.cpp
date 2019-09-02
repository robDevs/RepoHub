#include "user.h"

#include "search.h"
#include "json-tools.h"
#include "draw_templates.h"

User::User(std::string name, std::string avatar_url) {
    setName(name);
    haveInfo = false;
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

void User::setRname(std::string Rname) {
    this->Rname = Rname;
}
void User::setType(std::string type) {
    this->type = type;
}
void User::setCompany(std::string company) {
    this->company = company;
}
void User::setLocation(std::string location) {
    this->location = location;
}
void User::setEmail(std::string email) {
    this->email = email;
}
void User::setBio(std::string bio) {
    this->bio = bio;
}

void User::setAvatarUrl(std::string url) {
    this->avatar_url = url;
}

int User::getNumRepos() {
    return this->numRepos;
}

void User::setRepos() {
    std::string url = "https://api.github.com/users/";
    url += name;
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
    if(!haveInfo) {
        std::string url = "https://api.github.com/users/";
        url += name;

        jansson_parse_user_info(curl_get_string(url), this);
        haveInfo = true;
    }

    std::string check_follow_url = "https://api.github.com/user/following/";
    check_follow_url += name;

    bool following = false;

    if(authed)
        following = curl_check_star(check_follow_url);

    strip_carriage_return(bio);

    int cursor_pos = 0;

    Button tempButton;
    std::vector<Button> buttons; // the container for the buttons.
    //User a temp button and push it into the container.
    tempButton.name = "View repos";
    tempButton.x = 960 - 230;
    tempButton.y = 200;
    tempButton.w = 200;
    tempButton.h = 50;
    buttons.push_back(tempButton);

    if(authed) {
        tempButton.name = "Follow";
        tempButton.y = 260;
        buttons.push_back(tempButton);
    }

    std::string header_string = "Following/";
    header_string += name;

    bool done = false;

    init_keys();

    while(!done) {
        update_keys();

        if(up_released)
            cursor_pos -= 1;
        if(down_released)
            cursor_pos += 1;
        if(cursor_pos < 0)
            cursor_pos = 1;
        if(cursor_pos > 1)
            cursor_pos = 0;

        if(cross_released) {
            switch (cursor_pos) {
                case 0:
                    drawReposView();
                    break;
                case 1:
                    if(authed) {
                        if(following) {
                            curl_post_star(check_follow_url, true);
                            following = false;
                            draw_alert_message("You are no longer following this user. \nThis change will take effect next time you launch the app");
                        }
                        else {
                            curl_post_star(check_follow_url, false);
                            following = true;
                            draw_alert_message("You are now following this user. \nThis change will take effect next time you launch the app");
                        }
                    }
                    break;
                default:
                    break;
            }
        }

        if(circle_released)
            done = true;

        if(authed) {
            if(following)
                buttons[1].name = "Unfollow";
            else
                buttons[1].name = "Follow";
        }
        else cursor_pos = 0;

        vita2d_start_drawing();
        vita2d_clear_screen();

        //draw the template containing a sub header area and buttons area.
        drawTemplate(SUB_HEADER_BUTTONS, header_string, buttons, cursor_pos);

        //sub header goes over template. may add to template.
        vita2d_font_draw_text(font40, 40, 103 - 9, RGBA8(0,0,0,255), 40.0f, name.c_str());

        int rname_y, type_y, company_y, email_y, bio_y;

        rname_y = type_y = company_y = email_y = bio_y = 103 + 40;

        if(!Rname.empty()) {
            vita2d_font_draw_text(font20, 40, rname_y, RGBA8(0,0,0,255), 20.0f, "Name");
            vita2d_font_draw_textf(font20, 40, rname_y + 20, RGBA8(0,0,0,255), 20.0f, "- %s", Rname.c_str());
        }
        else rname_y = 103;

        if(!type.empty()) {
            type_y = rname_y + 40;
            vita2d_font_draw_text(font20, 40, type_y, RGBA8(0,0,0,255), 20.0f, "Type");
            vita2d_font_draw_textf(font20, 40, type_y + 20, RGBA8(0,0,0,255), 20.0f, "- %s", type.c_str());
        }
        else type_y = rname_y;

        if(!company.empty()) {
            company_y = type_y + 40;
            vita2d_font_draw_text(font20, 40, company_y, RGBA8(0,0,0,255), 20.0f, "Company");
            vita2d_font_draw_textf(font20, 40, company_y + 20, RGBA8(0,0,0,255), 20.0f, "- %s", company.c_str());
        }
        else company_y = type_y;

        if(!email.empty()) {
            email_y = company_y + 40;
            vita2d_font_draw_text(font20, 40, email_y, RGBA8(0,0,0,255), 20.0f, "Email");
            vita2d_font_draw_textf(font20, 40, email_y + 20, RGBA8(0,0,0,255), 20.0f, "- %s", email.c_str());
        }
        else email_y = company_y;

        if(!bio.empty()) {
            bio_y = email_y + 40;
            vita2d_font_draw_text(font20, 40, bio_y, RGBA8(0,0,0,255), 20.0f, "Bio");
            vita2d_font_draw_textf(font20, 40, bio_y + 20, RGBA8(0,0,0,255), 20.0f, "- %s", word_wrap(bio, 50).c_str());
        }
        else bio_y = email_y;

        vita2d_end_drawing();
        vita2d_common_dialog_update();
        vita2d_swap_buffers();
    }
}

void User::drawReposView() {
    if(!complete) {
        setRepos();
    }

    int cursor_pos = 0;
    int cursor_row = 0;
    int y_offset = 44;

    int list_size0 = static_cast<int>(repos_row0.size());
    int list_size1 = static_cast<int>(repos_row1.size());

    float menuBarH;

    if(list_size0 + list_size1 < numRepos)
        menuBarH = pow(500,2)/((list_size0+1)*140);
    else
        menuBarH = pow(500,2)/(list_size0*140);

    std::string header_string = "Following/";
    header_string += name;
    header_string += "/repositories";

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
                if(list_size0 + list_size1 < numRepos)
                    menuBarH = pow(500,2)/((list_size0+1)*140);
                else
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

        if(menuBarH < 500)
            vita2d_draw_rectangle(960 - 15, 44 - (y_offset)*(menuBarH/500), 15, menuBarH, RGBA8(167,167,167,255));

        draw_header(header_string);

        vita2d_end_drawing();
        vita2d_common_dialog_update();
        vita2d_swap_buffers();
    }
    reset_keys();
}

void User::drawStarred() {
    if(!starred_complete) {
        setStarred(1);
    }

    int cursor_pos = 0;
    int cursor_row = 0;
    int y_offset = 103;

    int list_size0 = static_cast<int>(starred_row0.size());
    int list_size1 = static_cast<int>(starred_row1.size());

    float menuBarH = pow(441,2)/((list_size0+1)*140);

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
                menuBarH = pow(441,2)/((list_size0+1)*140);
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

        if (rt_released || lt_released){
            break;
        }

        while(y_offset + (cursor_pos*140) > 544 - 140) {
            y_offset -= 140;
        }

        while(y_offset + (cursor_pos*140) < 103) {
            y_offset += 140;
        }

        vita2d_start_drawing();
        vita2d_clear_screen();

        for(int i = 0; i < list_size0; i++) {
            starred_row0[i].drawListView(40, y_offset + i*140, cursor_pos == i && cursor_row == 0, true);
        }

        for(int i = 0; i < list_size1; i++) {
            starred_row1[i].drawListView(500, y_offset + i*140, cursor_pos == i && cursor_row == 1, true);
            //draw_button(std::to_string(list_size1) + std::to_string(i), 500, y_offset + i*140, 420, 140, true);
        }

        if(numStarred + 1 > list_size0 + list_size1) {
            draw_button("Refresh", 40, y_offset + list_size0*140 + 40, 880, 50, cursor_pos == list_size0);
        }

        if(menuBarH < 441)
            vita2d_draw_rectangle(960 - 15, 103 - (y_offset-140+40)*(menuBarH/441), 15, menuBarH, RGBA8(167,167,167,255));

        vita2d_draw_rectangle(0, 44, 960, 103-44, RGBA8(255,255,255,255));
        vita2d_font_draw_textf(font40, 960-960/4-vita2d_font_text_width(font40, 40.0f, "Starred")/2, 95, RGBA8(0,0,0,255), 40.0f, "Starred");
        vita2d_font_draw_textf(font40, 960/4-vita2d_font_text_width(font40, 40.0f, "Following")/2, 95, RGBA8(150,150,150,255), 40.0f, "Following");
        vita2d_draw_line(0, 103, 960, 103, RGBA8(150,150,150,200));
        vita2d_draw_line(960/2, 44, 960/2, 103, RGBA8(150,150,150,200));

        draw_header("Home");

        vita2d_end_drawing();
        vita2d_common_dialog_update();
        vita2d_swap_buffers();
    }
}

void User::setStarred(int page) {
    starred_complete = true;
    std::string url = "https://api.github.com/users/";
    url += name;
    url += "/starred?sort=updated&direction=desc&page=";
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

    if(clear && !user_name.empty()) {
        User self(user_name, "");
        user_list->push_back(self);
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

    check_core_rate_limit();

    for(size_t i = 0; i < userNames.size(); i++) {
        User newUser(userNames[i], avatar_urls[i]);
        user_list->push_back(newUser);
    }
}


void draw_user_list(std::vector<User> *user_list, int *status) {
    int list_size = static_cast<int>(user_list->size());
    int cursor_pos = 0;
    int y_offset = 103;
    float menuBarH = 0;
    if(list_size < following_count)
        menuBarH = pow(441,2)/((list_size+1)*100);
    else
        menuBarH = pow(441,2)/(list_size*100);
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
                    if(!user_name.empty()) {
                        set_user_list(user_list, 1, true);
                        list_size = static_cast<int>(user_list->size());
                    }
                }
                if(state == UPDATE_TOKEN) {
                    set_token();
                    if(have_token){
                        jansson_get_authed_user(curl_get_string("https://api.github.com/user"), &user_name, &authed, &following_count);
                        check_core_rate_limit();
                    }
                    if(!authed)
                        have_token = false;
                    if(!user_name.empty()) {
                        set_user_list(user_list, 1, true);
                        list_size = static_cast<int>(user_list->size());
                    }
                }
                if(state == QUIT_APP) {
                    *status = QUIT_APP;
                    done = true;
                    state = MAIN_VIEW;
                }

                if(state == SUBMIT_ISSUE && authed) {
                    draw_issue_menu();
                }
                else if(state == SUBMIT_ISSUE && !authed)
                    draw_alert_message("Requires authentication!");

                for(int i = 0; i < 3; i++) {
                    vita2d_start_drawing();
                    vita2d_clear_screen();

                    for(int i = 0; i < list_size; i++) {
                        user_list->at(i).drawListView(y_offset + i*100, cursor_pos == i);
                    }
                    if(menuBarH < 500)
                        vita2d_draw_rectangle(960 - 15, 44 - (y_offset-100)*(menuBarH/500), 15, menuBarH, RGBA8(167,167,167,255));

                    vita2d_draw_rectangle(0, 44, 960, 103-44, RGBA8(255,255,255,255));
                    vita2d_font_draw_textf(font40, 960-960/4-vita2d_font_text_width(font40, 40.0f, "Starred")/2, 95, RGBA8(150,150,150,255), 40.0f, "Starred");
                    vita2d_font_draw_textf(font40, 960/4-vita2d_font_text_width(font40, 40.0f, "Following")/2, 95, RGBA8(0,0,0,255), 40.0f, "Following");
                    vita2d_draw_line(0, 103, 960, 103, RGBA8(150,150,150,200));
                    vita2d_draw_line(960/2, 44, 960/2, 103, RGBA8(150,150,150,200));

                    draw_header("Home");

                    vita2d_end_drawing();
                    vita2d_wait_rendering_done();
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
                    if(list_size < following_count)
                        menuBarH = pow(441,2)/((list_size+1)*100);
                    else
                        menuBarH = pow(441,2)/(list_size*100);
                }
                else
                    user_list->at(cursor_pos).drawDetailsView();
            }
        }

        if(rt_released || lt_released) {
            if(list_size > 0) {
                user_list->at(0).drawStarred();
            }
        }

        while(y_offset + (cursor_pos*100) > 544 - 100) {
            y_offset -= 100;
        }

        while(y_offset + (cursor_pos*100) < 103) {
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
        if(menuBarH < 441)
            vita2d_draw_rectangle(960 - 15, 103-(y_offset-100)*(menuBarH/441), 15, menuBarH, RGBA8(167,167,167,255));

        vita2d_draw_rectangle(0, 44, 960, 103-44, RGBA8(255,255,255,255));
        vita2d_font_draw_textf(font40, 960-960/4-vita2d_font_text_width(font40, 40.0f, "Starred")/2, 95, RGBA8(150,150,150,255), 40.0f, "Starred");
        vita2d_font_draw_textf(font40, 960/4-vita2d_font_text_width(font40, 40.0f, "Following")/2, 95, RGBA8(0,0,0,255), 40.0f, "Following");
        vita2d_draw_line(0, 103, 960, 103, RGBA8(150,150,150,200));
        vita2d_draw_line(960/2, 44, 960/2, 103, RGBA8(150,150,150,200));

        draw_header("Home");

        if(list_size == 0)
            vita2d_font_draw_text(font40, 960/2-vita2d_font_text_width(font40, 40.0f, "Press start to get started!")/2, 544 / 2, RGBA8(150,150,150,255), 40.0f, "Press start to get started!");

        vita2d_end_drawing();
        vita2d_common_dialog_update();
        vita2d_swap_buffers();
    }
}
