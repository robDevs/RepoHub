#include "search.h"
#include "curl-tools.h"
#include "json-tools.h"
#include "user.h"

void search() {
    int cursor_pos = 0;

    bool done = false;

    std::string user_query = "";
    std::string repo_query = "";
    std::string keyWord_query = "";

    while(!done) {
        update_keys();

        //temp quit option.
        if(start_released)
            done = true;
        //move cursor
        if(up_released)
            cursor_pos -= 1;
        if(down_released)
            cursor_pos += 1;
        //check if cursor is out of bounds.
        if(cursor_pos > 4)
            cursor_pos = 0;
        if(cursor_pos < 0)
            cursor_pos = 4;

        if(circle_released)
            done = true;

        //if cross is released check cursor pos and act accordingly.
        if(cross_released) {
            switch (cursor_pos) {
                case 0:
                    user_query = vita_keyboard_get((char*)"Enter a user name", (const char*)user_query.c_str(), 600, 0);
                    break;
                case 1:
                    repo_query = vita_keyboard_get((char*)"Enter a repo name", (const char*)repo_query.c_str(), 600, 0);
                    break;
                case 2:
                    keyWord_query = vita_keyboard_get((char*)"Enter a keyword", (const char*)keyWord_query.c_str(), 600, 0);
                    for(size_t i = 0; i < keyWord_query.size(); i++)
                        if(keyWord_query[i] == ' ')
                            keyWord_query[i] = '+';
                    break;
                case 3:
                    if(!user_query.empty() && repo_query.empty())
                        user_search(user_query);
                    else if(!user_query.empty() && !repo_query.empty()) {
                        std::string final_search = "repo:";
                        final_search += user_query;
                        final_search += "/";
                        final_search += repo_query;
                        final_search += "+fork:true";
                        repo_search(final_search);
                    }
                    else if(user_query.empty() && !repo_query.empty()) {
                        std::string final_search = repo_query;
                        if(!keyWord_query.empty()) {
                            final_search += "+";
                            final_search += keyWord_query;
                            final_search += "+in:name,description";
                        }
                        final_search += "+fork:true";
                        repo_search(final_search);
                    }
                    else if(user_query.empty() && repo_query.empty() && !keyWord_query.empty()) {
                        std::string final_search = keyWord_query;
                        final_search += "+fork:true";
                        repo_search(keyWord_query);
                    }
                    break;
                case 4:
                    done = true;
            }
        }

        vita2d_start_drawing();
        vita2d_clear_screen();

        //right hand menu bar
        vita2d_draw_rectangle(960 - 260, 103, 260, 544 - 103, RGBA8(255,255,255,255));
        vita2d_draw_line(960-260, 104, 960-260, 544, RGBA8(150,150,150,200));
        draw_button("Edit User name", 960 - 230, 200, 200, 50, cursor_pos == 0);
        draw_button("Edit Repo name", 960 - 230, 260, 200, 50, cursor_pos == 1);
        draw_button("Edit Keyword", 960 - 230, 320, 200, 50, cursor_pos == 2);
        draw_button("Search", 960 - 230, 380, 200, 50, cursor_pos == 3);
        draw_button("Cancel", 960 - 230, 440, 200, 50, cursor_pos == 4);

        //bg and line for title under header
        vita2d_draw_rectangle(0, 44, 960, 103-44, RGBA8(255,255,255,255));
        vita2d_draw_line(0, 103, 960, 103, RGBA8(150,150,150,200));
        vita2d_font_draw_text(font40, 40, 103 - 9, RGBA8(0,0,0,255), 40.0f, "Enter search terms:");

        //draw search terms
        vita2d_font_draw_text(font20, 40, 103 + 80 - 5, RGBA8(0,0,0,255), 20.0f, "User Name");
        draw_empty_rectangle(40, 103 + 80, 960-260-80, 30, RGBA8(0,0,0,255));
        vita2d_font_draw_text(font20, 45, 103 + 80 + 20, RGBA8(0,0,0,255), 20.0f, user_query.c_str());

        vita2d_font_draw_text(font20, 40, 103 + (544-104)/2-15 - 5, RGBA8(0,0,0,255), 20.0f, "Repo Name");
        draw_empty_rectangle(40, 103 + (544-104)/2-15, 960-260-80, 30, RGBA8(0,0,0,255));
        vita2d_font_draw_text(font20, 45, 103 + (544-104)/2-15 + 20, RGBA8(0,0,0,255), 20.0f, repo_query.c_str());

        vita2d_font_draw_text(font20, 40, 544 - 110 - 5, RGBA8(0,0,0,255), 20.0f, "Keyword (only if user name is empty)");
        draw_empty_rectangle(40, 544 - 110, 960-260-80, 30, RGBA8(0,0,0,255));
        vita2d_font_draw_text(font20, 45, 544 - 110 + 20, RGBA8(0,0,0,255), 20.0f, keyWord_query.c_str());

        draw_header("Search");

        vita2d_end_drawing();
        vita2d_common_dialog_update();
        vita2d_swap_buffers();
    }
    reset_keys();
}

extern void user_search(std::string query) {
    std::string final_query = "https://api.github.com/search/users?q=";
    final_query += query;

    std::string result = curl_get_string(final_query);

    std::vector<std::string> userList;
    jansson_parse_user_search(result, &userList);

    std::vector<User> users;

    for(size_t i = 0; i < userList.size(); i++) {
        User newUser(userList[i], "");
        users.push_back(newUser);
    }

    int list_size = static_cast<int>(userList.size());

    int cursor_pos = 0;
    int y_offset = 44;
    float menuBarH = 0;

    menuBarH = pow(500,2)/(list_size*100);

    bool done = false;

    while(!done) {
        update_keys();

        if (down_released) {
            if(cursor_pos < list_size - 1) cursor_pos += 1;
            else cursor_pos = 0;
        }

        if (up_released) {
            if(cursor_pos > 0) cursor_pos -= 1;
            else cursor_pos = list_size - 1;
        }

        if(cross_released) {
            if(list_size > 0) {
                users[cursor_pos].drawDetailsView();
            }
        }

        if(circle_released)
            done = true;

        while(y_offset + (cursor_pos*100) > 544 - 100) {
            y_offset -= 100;
        }

        while(y_offset + (cursor_pos*100) < 44) {
            y_offset += 100;
        }

        vita2d_start_drawing();
        vita2d_clear_screen();

        for(int i = 0; i < list_size; i++) {
            users[i].drawListView(y_offset+i*100, cursor_pos == i);
        }

        if(menuBarH < 500)
            vita2d_draw_rectangle(960 - 15, 44-(y_offset)*(menuBarH/500), 15, menuBarH, RGBA8(167,167,167,255));

        draw_header("Search Results");

        vita2d_end_drawing();
        vita2d_common_dialog_update();
        vita2d_swap_buffers();
    }
}
extern void repo_search(std::string query) {
    std::string final_query = "https://api.github.com/search/repositories?q=";
    final_query += query;

    std::string result = curl_get_string(final_query);
    std::vector<Repo> repoList0, repoList1;

    int numRepos = jansson_parse_repo_search(result, &repoList0, &repoList1);

    bool done = false;

    int cursor_pos = 0;
    int cursor_row = 0;

    int list_size0 = static_cast<int>(repoList0.size());
    int list_size1 = static_cast<int>(repoList1.size());

    int y_offset = 44;

    float menuBarH;

    if(list_size0 + list_size1 < numRepos)
        menuBarH = pow(500,2)/((list_size0+1)*140);
    else
        menuBarH = pow(500,2)/(list_size0*140);

    while(!done) {

        update_keys();

        if(circle_released)
            done = true;

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

        while(y_offset + (cursor_pos*140) > 544 - 140) {
            y_offset -= 140;
        }

        while(y_offset + (cursor_pos*140) < 44) {
            y_offset += 140;
        }

        if(cross_released) {
            if(numRepos > 0) {
                if(cursor_row == 0)
                    repoList0[cursor_pos].drawDetailsView("Search Results");
                else if(cursor_row == 1)
                    repoList1[cursor_pos].drawDetailsView("Search Results");
            }
        }

        vita2d_start_drawing();
        vita2d_clear_screen();

        for(int i = 0; i < list_size0; i++) {
            repoList0[i].drawListView(40, y_offset + i*140, cursor_pos == i && cursor_row == 0, true);
        }

        for(int i = 0; i < list_size1; i++) {
            repoList1[i].drawListView(500, y_offset + i*140, cursor_pos == i && cursor_row == 1, true);
            //draw_button(std::to_string(list_size1) + std::to_string(i), 500, y_offset + i*140, 420, 140, true);
        }

        if(menuBarH < 500)
            vita2d_draw_rectangle(960 - 15, 44 - (y_offset)*(menuBarH/500), 15, menuBarH, RGBA8(167,167,167,255));

        draw_header("Search Results");

        vita2d_end_drawing();
        vita2d_common_dialog_update();
        vita2d_swap_buffers();
    }
    reset_keys();
}
