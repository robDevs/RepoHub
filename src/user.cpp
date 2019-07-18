#include "user.h"

User::User(std::string name) {
    setName(name);
    complete = false;

    setRepos();
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
    url += "/repos";

    std::string json_repo_list_string = curl_get_string(url);

    int count = jansson_parse_repo_list(json_repo_list_string, &repos);
    setNumRepos(count);

    for(size_t i = 0; i < repos.size(); i++) {
        repos[i].setOwner(name);
    }

    complete = true;
}

void User::drawListView(int x, int y) {
    vita2d_draw_rectangle(x - 20, y, 20, 100, RGBA8(255,0,0,255));
    vita2d_draw_rectangle(x, y, 960, 100, RGBA8(255,255,255,255));
    vita2d_font_draw_textf(font40, x, y+50+(vita2d_font_text_height(font40, 40.0f, name.c_str()) / 2), RGBA8(0,0,0,255), 40.0f, "%s", name.c_str());
    vita2d_draw_line(x, y, x + 960, y, RGBA8(150,150,150,150));
    vita2d_draw_line(x, y + 100, x + 960, y + 100, RGBA8(150,150,150,150));
}

void User::drawDetailsView() {
    //coming soon

    if(!complete) {
        setRepos();
    }

    int cursor_pos = 0;
    int y_offset = 44;

    float menuBarH = pow(522,2)/(numRepos*100);

    std::string header_string = name;
    header_string += "'s Repositories";

    init_keys();

    while(1) {
        update_keys();
        if (down_released) {
            if(cursor_pos < numRepos - 1) cursor_pos += 1;
            else cursor_pos = 0;
        }

        if (up_released) {
            if(cursor_pos > 0) cursor_pos -= 1;
            else cursor_pos = numRepos - 1;
        }

        if(cross_released) {
            repos[cursor_pos].drawDetailsView();
        }

        if (circle_released){
            break;
        }

        if(y_offset + (cursor_pos*100) > 544 - 100) {
            y_offset -= 100;
        }

        if(y_offset + (cursor_pos*100) < 44) {
            y_offset += 100;
        }

        vita2d_start_drawing();
        vita2d_clear_screen();

        for(int i = 0; i < numRepos; i++) {
            int x;
            if(cursor_pos == i) x = 20;
            else x = 0;
            repos[i].drawListView(x,y_offset + i*100);
        }

        vita2d_draw_rectangle(960 - 10, 44 - y_offset*(menuBarH/522), 5, menuBarH, RGBA8(36,41,46,255));

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

    float menuBarH = pow(544,2)/(list_size*100);

    //Rectangle posRect = {screenWidth - 10, 0 - listPos*(menuBarH/screenHeight), 5, menuBarH};

    init_keys();

    while(1) {
        update_keys();

        if (start_released){
            *status = QUIT_APP;
            break;
        }

        if (select_released) {
            
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
            int x;
            if(cursor_pos == i) x = 20;
            else x = 0;
            user_list[i].drawListView(x,y_offset + i*100);
        }
        vita2d_draw_rectangle(960 - 10, 44 - y_offset*(menuBarH/544), 5, menuBarH, RGBA8(36,41,46,255));

        draw_header("Saved Users");

        vita2d_end_drawing();
        vita2d_common_dialog_update();
        vita2d_swap_buffers();
    }
}
