#include "user.h"

User::User(std::string name) {
    setName(name);
    complete = false;
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

    url += "/repos?per_page=100";

    std::string json_repo_list_string = curl_get_string(url);
    jannson_get_rate_limits(curl_get_string("https://api.github.com/rate_limit"), &core_max, &core_remain, &search_max, &search_remain);

    check_core_rate_limit();

    jansson_parse_repo_list(json_repo_list_string, &repos_row0, &repos_row1);

    for(size_t i = 0; i < repos_row0.size(); i++) {
        repos_row0[i].setOwner(name);
    }
    for(size_t i = 0; i < repos_row1.size(); i++) {
        repos_row1[i].setOwner(name);
    }

    complete = true;
}

void User::drawListView(int y, bool selected) {
    draw_list_item(name, "", "", y, selected);
}

void User::drawDetailsView() {
    //coming soon

    if(!complete) {
        setRepos();
    }

    int cursor_pos = 0;
    int cursor_row = 0;
    int y_offset = 44;

    int list_size0 = static_cast<int>(repos_row0.size());
    int list_size1 = static_cast<int>(repos_row1.size());

    float menuBarH = pow(500,2)/(list_size0*140);

    std::string header_string = "Saved users->";
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
                    url += "&per_page=100";

                    jansson_parse_repo_list(curl_get_string(url), &repos_row0, &repos_row1);

                    jannson_get_rate_limits(curl_get_string("https://api.github.com/rate_limit"), &core_max, &core_remain, &search_max, &search_remain);

                    check_core_rate_limit();

                    for(size_t i = 0; i < repos_row0.size(); i++) {
                        repos_row0[i].setOwner(name);
                    }
                    for(size_t i = 0; i < repos_row1.size(); i++) {
                        repos_row1[i].setOwner(name);
                    }

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

        vita2d_draw_rectangle(960 - 10, 44 - y_offset*(menuBarH/500), 5, menuBarH, RGBA8(36,41,46,255));

        draw_header(header_string);

        vita2d_end_drawing();
        vita2d_common_dialog_update();
        vita2d_swap_buffers();
    }
}

//end class functions

void draw_user_list(std::vector<User> user_list, int *status) {
    int list_size = static_cast<int>(user_list.size());
    int cursor_pos = 0;
    int y_offset = 44;

    float menuBarH = pow(500,2)/(list_size*100);

    //Rectangle posRect = {screenWidth - 10, 0 - listPos*(menuBarH/screenHeight), 5, menuBarH};

    init_keys();

    while(1) {
        update_keys();

        if (start_released){
            *status = QUIT_APP;
            break;
        }

        if (select_released) {
            get_token();
        }

        if (down_released) {
            if(cursor_pos < list_size - 1) cursor_pos += 1;
            else cursor_pos = 0;
        }

        if (up_released) {
            if(cursor_pos > 0) cursor_pos -= 1;
            else cursor_pos = list_size - 1;
        }

        if(cross_released) {
            user_list[cursor_pos].drawDetailsView();
        }

        if(y_offset + (cursor_pos*100) > 544 - 100) {
            y_offset -= 100;
        }

        if(y_offset + (cursor_pos*100) < 44) {
            y_offset += 100;
        }

        vita2d_start_drawing();
        vita2d_clear_screen();

        for(int i = 0; i < list_size; i++) {
            user_list[i].drawListView(y_offset + i*100, cursor_pos == i);
        }
        vita2d_draw_rectangle(960 - 10, 44 - y_offset*(menuBarH/500), 5, menuBarH, RGBA8(36,41,46,255));

        draw_header("Saved users");

        vita2d_end_drawing();
        vita2d_common_dialog_update();
        vita2d_swap_buffers();
    }
}
