#include "draw.h"
#include <psp2/kernel/processmgr.h>
#include <psp2/rtc.h>

#include "vita-keyboard.h"
#include "curl-tools.h"
#include "search.h"
#include "draw_templates.h"

void draw_header(std::string header) {
    SceDateTime time;
    sceRtcGetCurrentClockLocalTime(&time);

    std::string am_pm;

    std::string time_string = "";
    int time_hour = sceRtcGetHour(&time);
    if(time_hour > 12) {
        time_hour -= 12;
        am_pm = " PM";
    }
    else {
        am_pm = " AM";
    }
    if(time_hour == 0)
        time_hour = 12;
    int time_minute = sceRtcGetMinute(&time);
    time_string += std::to_string(time_hour);
    time_string += ":";
    if(time_minute < 10) time_string += "0";
    time_string += std::to_string(time_minute);
    time_string += am_pm;
    time_string += " | API:";
    time_string += std::to_string(core_remain);
    time_string += "/";
    time_string += std::to_string(core_max);
    vita2d_draw_rectangle(0, 0, 960, 44, RGBA8(36,41,46,255));
    vita2d_font_draw_textf(font20, 20, 22+(vita2d_font_text_height(font20, 20.0f, header.c_str()) / 4), RGBA8(255,255,255,255), 20.0f, "%s", header.c_str());

    vita2d_font_draw_textf(font20, 960-(vita2d_font_text_width(font20, 20.0f, time_string.c_str())) - 20, 22+(vita2d_font_text_height(font20, 20.0f, header.c_str()) / 4), RGBA8(255,255,255,255), 20.0f, "%s", time_string.c_str());
}

void draw_button(std::string name, int x, int y, int w, int h, bool selected) {
    if(selected) {
        vita2d_draw_rectangle(x-2, y-2, w + 4, h+4, RGBA8(36,41,46,255));
        vita2d_draw_rectangle(x, y, w, h, RGBA8(255,255,255, 255));
        vita2d_font_draw_textf(font20, x+w/2 - (vita2d_font_text_width(font20, 20.0f, name.c_str()) / 2), y + h/2+vita2d_font_text_height(font20, 20.0f, name.c_str()) / 4, RGBA8(0,0,0,255), 20.0f, "%s", name.c_str());
    }
    else {
        vita2d_draw_rectangle(x-1, y-1, w + 2, h + 2, RGBA8(150,150,150,200));
        vita2d_draw_rectangle(x, y, w, h, RGBA8(255,255,255, 255));
        vita2d_font_draw_textf(font20, x+w/2 - (vita2d_font_text_width(font20, 20.0f, name.c_str()) / 2), y + h/2+vita2d_font_text_height(font20, 20.0f, name.c_str()) / 4, RGBA8(150,150,150,200), 20.0f, "%s", name.c_str());
    }
}

void draw_button(Button button, bool selected) {
    draw_button(button.name, button.x, button.y, button.w, button.h, selected);
}

void draw_list_item(std::string name, std::string body, std::string language, int x, int y, bool selected) {
    vita2d_draw_rectangle(0, y, 960, 100, RGBA8(255,255,255,255));
    vita2d_font_draw_textf(font40, x, y+50+(vita2d_font_text_height(font40, 40.0f, name.c_str()) / 4), RGBA8(3,102,214,255), 40.0f, "%s", name.c_str());
    vita2d_font_draw_textf(font15, x + 10, y+50+(vita2d_font_text_height(font40, 40.0f, name.c_str())), RGBA8(88, 96, 105,255), 15.0f, "%s", body.c_str());

    if(selected) {
        vita2d_draw_line(0, y, 960, y, RGBA8(36,41,46,255));
        vita2d_draw_line(0, y+1, 960, y+1, RGBA8(36,41,46,255));
        vita2d_draw_line(0, y + 100, 960, y + 100, RGBA8(36,41,46,255));
        vita2d_draw_line(0, y + 100 - 1, 960, y + 100 - 1, RGBA8(36,41,46,255));
    }
    else {
        vita2d_draw_line(0, y, 960, y, RGBA8(150,150,150,200));
        vita2d_draw_line(0, y + 100, 960, y + 100, RGBA8(150,150,150,200));
    }
}

void draw_repo_list_item(std::string name, std::string body, std::string language, int stars, int forks, std::string updated_at, bool fork, int x, int y, bool selected) {
    if(body.size() > 60) {
        body.resize(60);
        body += "...";
    }

    if(name.size() > 28) {
        name.resize(28);
        name += "...";
    }

    if(updated_at.size() > 10)
        updated_at.resize(10);

    int lang_circle_x = x+20+8;
    std::string stars_final = std::to_string(stars);
    std::string forks_final = std::to_string(forks);
    int language_x = lang_circle_x + 10;
    int star_icon_x = language_x + vita2d_font_text_width(font15, 15.0f, language.c_str()) + 5;
    if(language.empty())
        star_icon_x = x + 20;
    int stars_x = star_icon_x + 15;
    int forks_icon = stars_x + vita2d_font_text_width(font15, 15.0f, stars_final.c_str()) + 5;
    if(stars < 1)
        forks_icon = star_icon_x;
    int forks_x = forks_icon + 15;

    if(selected) {
        vita2d_draw_rectangle(x-2, y+3, 424, 144, RGBA8(255,255,255,255));
        vita2d_draw_rectangle(x-2, y+40-2, 424, 104, RGBA8(36,41,46,255));
        vita2d_draw_rectangle(x, y+40, 420, 100, RGBA8(255,255,255,255));
    }
    else {
        vita2d_draw_rectangle(x, y + 3, 424, 144, RGBA8(255,255,255,255));
        vita2d_draw_rectangle(x-2, y+40-2, 424, 104, RGBA8(150,150,150,200));
        vita2d_draw_rectangle(x, y+40, 420, 100, RGBA8(255,255,255,255));
    }
    if(y + 106 > 0 && y < 544) {
        if(fork) {
            vita2d_draw_texture_scale(fork_tex, x+20, y+40+15, 0.2188, 0.2188);
            vita2d_font_draw_text(font20, x+35, y+40+25+(vita2d_font_text_height(font20, 20.0f, name.c_str()) / 4), RGBA8(3,102,214,255), 20.0f, name.c_str());
        }
        else {
            vita2d_font_draw_text(font20, x+20, y+40+25+(vita2d_font_text_height(font20, 20.0f, name.c_str()) / 4), RGBA8(3,102,214,255), 20.0f, name.c_str());
        }

        vita2d_font_draw_textf(font15, x+20, y+40+25+(vita2d_font_text_height(font20, 20.0f, name.c_str())) + 5, RGBA8(88, 96, 105,255), 15.0f, "%s", word_wrap(body, 40).c_str());

        if(!language.empty()) {
            vita2d_draw_fill_circle(lang_circle_x, y+40+100-15, 7, get_lang_color(language));
            vita2d_font_draw_textf(font15, language_x, y+40+100-10, RGBA8(88, 96, 105,255), 15.0f, language.c_str());
        }
        if(stars > 0) {
            vita2d_font_draw_textf(font15, stars_x, y+40+100-10, RGBA8(88, 96, 105,255), 15.0f, stars_final.c_str());
            vita2d_draw_texture_scale(star, star_icon_x, y+40+100-23, 0.2188, 0.2188);
        }
        if(forks > 0) {
            vita2d_font_draw_textf(font15, forks_x, y+40+100-10, RGBA8(88, 96, 105,255), 15.0f, forks_final.c_str());
            vita2d_draw_texture_scale(fork_tex, forks_icon, y+40+100-23, 0.2188, 0.2188);
        }


        std::string updated_at_final = "Updated at: ";
        updated_at_final += updated_at;
        vita2d_font_draw_text(font15, x + 420 - vita2d_font_text_width(font15, 15.0f, updated_at_final.c_str()) - 20, y+40+100-10, RGBA8(88, 96, 105,255), 15.0f, updated_at_final.c_str());

    }
}

void draw_list_item_small(std::string name, int y, bool selected) {
    vita2d_draw_rectangle(0, y, 960, 100, RGBA8(255,255,255,255));
    vita2d_font_draw_textf(font20, 40, y+50+(vita2d_font_text_height(font20, 20.0f, name.c_str()) / 4), RGBA8(0,0,0,255), 20.0f, "%s", name.c_str());
    if(selected) {
        vita2d_draw_line(0, y, 960, y, RGBA8(36,41,46,255));
        vita2d_draw_line(0, y+1, 960, y+1, RGBA8(36,41,46,255));
        vita2d_draw_line(0, y + 100, 960, y + 100, RGBA8(36,41,46,255));
        vita2d_draw_line(0, y + 100 - 1, 960, y + 100 - 1, RGBA8(36,41,46,255));
    }
    else {
        vita2d_draw_line(0, y, 960, y, RGBA8(150,150,150,200));
        vita2d_draw_line(0, y + 100, 960, y + 100, RGBA8(150,150,150,200));
    }
}

void draw_release_list_item(std::string name, std::string tag_name, std::string author, std::string published_at, int x, int y, bool selected) {
    author += " published this at: ";
    published_at.resize(10);
    author += published_at;
    vita2d_draw_rectangle(0, y, 960, 100, RGBA8(255,255,255,255));
    vita2d_font_draw_textf(font20, x, y+20+(vita2d_font_text_height(font20, 20.0f, name.c_str()) / 4), RGBA8(3,102,214,255), 20.0f, "%s", name.c_str());
    vita2d_font_draw_textf(font15, x + 10, y+50+7, RGBA8(88, 96, 105,255), 15.0f, "%s", tag_name.c_str());
    vita2d_font_draw_textf(font15, x + 10, y+85, RGBA8(88, 96, 105,255), 15.0f, "%s", author.c_str());

    if(selected) {
        vita2d_draw_line(0, y, 960, y, RGBA8(36,41,46,255));
        vita2d_draw_line(0, y+1, 960, y+1, RGBA8(36,41,46,255));
        vita2d_draw_line(0, y + 100, 960, y + 100, RGBA8(36,41,46,255));
        vita2d_draw_line(0, y + 100 - 1, 960, y + 100 - 1, RGBA8(36,41,46,255));
    }
    else {
        vita2d_draw_line(0, y, 960, y, RGBA8(150,150,150,200));
        vita2d_draw_line(0, y + 100, 960, y + 100, RGBA8(150,150,150,200));
    }
}

bool draw_yes_no_message(std::string message) {
    init_keys();

    bool return_val = false;
    bool done = false;
    int cursor_pos = 1;

    while(!done) {
        update_keys();
        if(cross_released) {
            if(cursor_pos == 0) return_val = true;
            else return_val = false;

            done = true;
        }

        if(circle_released) {
            return_val = false;
            done = true;
        }

        if(left_released) cursor_pos -= 1;
        if(right_released) cursor_pos += 1;

        if(cursor_pos < 0) cursor_pos = 1;
        if(cursor_pos > 1) cursor_pos = 0;

        vita2d_start_drawing();

        vita2d_draw_rectangle(0, 544 / 2 - 100, 960, 200, RGBA8(36,41,46,255));

        vita2d_font_draw_text(font20, (960/2)-(vita2d_font_text_width(font20, 20.0f, message.c_str()) / 2), 544 / 2 - 40, RGBA8(255,255,255,255), 20.0f, message.c_str());

        draw_button("Yes", 960 / 2 - 100, 544/2 + 25, 50, 50, cursor_pos == 0);
        draw_button("No", 960 / 2 + 50, 544/2 + 25, 50, 50, cursor_pos == 1);

        vita2d_end_drawing();
        vita2d_common_dialog_update();
		vita2d_swap_buffers();
    }
    reset_keys();
    return return_val;
}
void draw_alert_message(std::string message) {
    init_keys();

    bool done = false;

    while(!done) {
        update_keys();
        if(cross_released)
            done = true;
        if(circle_released)
            done = true;


        vita2d_start_drawing();

        vita2d_draw_rectangle(0, 544 / 2 - 100, 960, 200, RGBA8(36,41,46,255));

        vita2d_font_draw_text(font20, 960/2 - vita2d_font_text_width(font20, 20.0f, message.c_str()) / 2, 544 / 2 - 40, RGBA8(255,255,255,255), 20.0f, message.c_str());

        draw_button("Okay", 960 / 2 - 75 / 2, 544/2 + 25, 75, 50, true);

        vita2d_end_drawing();
        vita2d_common_dialog_update();
		vita2d_swap_buffers();
    }
    reset_keys();
}

void draw_alert_message_time(std::string message, int time) {
    for(int i = 0; i < time; i++) {
        vita2d_start_drawing();

        vita2d_draw_rectangle(0, 544 / 2 - 100, 960, 200, RGBA8(36,41,46,255));

        vita2d_font_draw_text(font20, 960/2 - vita2d_font_text_width(font20, 20.0f, message.c_str()) / 2, 544 / 2 - 40, RGBA8(255,255,255,255), 20.0f, message.c_str());

        vita2d_end_drawing();
        vita2d_common_dialog_update();
		vita2d_swap_buffers();
    }
}

void draw_extract_progress(std::string path, std::string fileName, double fileProgress, double fileTotal, double zipProgress, double zipTotal) {

    std::string pathString = "Extracting from: ";
    pathString += path;

    std::string nameString;
    std::string fileString ;
    std::string totalString;

    nameString = "extracting: ";
    nameString += fileName;

    fileString += "File: ";
    fileString += std::to_string(byte_to_mb(fileProgress));
    fileString += " MB / ";
    fileString += std::to_string(byte_to_mb(fileTotal));
    fileString += " MB";

    totalString = "Total: ";
    totalString += std::to_string(byte_to_mb(zipProgress));
    totalString += " MB / ";
    totalString += std::to_string(byte_to_mb(zipTotal));
    totalString += " MB";

    for(int i = 0; i < 1; i++) {
        vita2d_start_drawing();

        vita2d_draw_rectangle(0, 544 / 2 - 100, 960, 200, RGBA8(36,41,46,255));

        vita2d_font_draw_text(font20, 960/2 - vita2d_font_text_width(font20, 20.0f, pathString.c_str()) / 2, 544 / 2 - 60, RGBA8(255,255,255,255), 20.0f, pathString.c_str());

        vita2d_font_draw_text(font20, 960/2 - vita2d_font_text_width(font20, 20.0f, nameString.c_str()) / 2, 544 / 2 - 30, RGBA8(255,255,255,255), 20.0f, nameString.c_str());

        vita2d_font_draw_text(font20, (960/2)-(vita2d_font_text_width(font20, 20.0f, fileString.c_str()) / 2), 544 / 2 - 10, RGBA8(255,255,255,255), 20.0f, fileString.c_str());
        vita2d_font_draw_text(font20, (960/2)-(vita2d_font_text_width(font20, 20.0f, totalString.c_str()) / 2), 544 / 2 + 10, RGBA8(255,255,255,255), 20.0f, totalString.c_str());

        // how wide you want the progress meter to be
        int total=320;

        double fraction = 0;
        int progress = 0;
        if(fileTotal > 0.0) {
            fraction = fileProgress / fileTotal;
            progress = round(fraction * total);
            vita2d_draw_rectangle(960 / 2 - 160, 544 / 2 + 22, progress, 4, RGBA8(0,255,0,255));
        }

        fraction = 0;
        progress = 0;
        if(zipTotal > 0.0) {
          fraction = zipProgress / zipTotal;
          progress = round(fraction * total);
          vita2d_draw_rectangle(960 / 2 - 160, 544 / 2 + 28, progress, 4, RGBA8(0,255,0,255));
        }

        vita2d_end_drawing();
        vita2d_common_dialog_update();
		vita2d_swap_buffers();
    }
}

int draw_star_menu() {
    bool done = false;
    int return_val = MAIN_VIEW;
    int cursor_pos = 0;
    init_keys();

    while(!done) {
        update_keys();

        if(cross_released) {
            switch (cursor_pos) {
                case 0:
                    return_val = UPDATE_USERNAME;
                    done = true;
                    break;
                case 1:
                    return_val = UPDATE_TOKEN;
                    done = true;
                    break;
                case 2:
                    search();
                    return_val = MAIN_VIEW;
                    done = true;
                    goto quickEnd;
                    break;
                case 3:
                    return_val = SUBMIT_ISSUE;
                    done = true;
                    break;
                case 4:
                    return_val = MAIN_VIEW;
                    done = true;
                    break;
                case 5:
                    return_val = QUIT_APP;
                    done = true;
                    break;
            }
        }

        if(circle_released) {
            return_val = MAIN_VIEW;
            done = true;
        }

        if(up_released) cursor_pos -= 1;
        if(down_released) cursor_pos += 1;
        if(cursor_pos < 0) cursor_pos = 5;
        if(cursor_pos > 5) cursor_pos = 0;

        vita2d_start_drawing();

        vita2d_draw_rectangle(960 / 2 - 100, 544 / 2 - 155, 200, 310, RGBA8(36,41,46,255));

        draw_button("Enter UserName", 960 / 2 - 90, 544 / 2 - 145, 180, 40, cursor_pos == 0);
        draw_button("Enter Token", 960 / 2 - 90, 544 / 2 - 95, 180, 40, cursor_pos == 1);
        draw_button("Search", 960 / 2 - 90, 544 / 2 - 45, 180, 40, cursor_pos == 2);
        draw_button("Submit Issue", 960 / 2 - 90, 544 / 2 + 5, 180, 40, cursor_pos == 3);
        draw_button("Return", 960 / 2 - 90, 544 / 2 + 55, 180, 40, cursor_pos == 4);
        draw_button("Quit", 960 / 2 - 90, 544 / 2 + 105, 180, 40, cursor_pos == 5);

        vita2d_end_drawing();
        vita2d_common_dialog_update();
		vita2d_swap_buffers();
    }

quickEnd:
    reset_keys();
    return return_val;
}

void draw_issue_menu() {
    init_keys();

    std::string title = "Title";
    std::string body = "Body";

    int cursor_pos = 0;
    int body_pos = 143;

    bool show_footer = false;
    std::string footer = "\n\n---\n_Submitted from RepoHub on PS Vita_";

    bool done = false;

    //create some buttons.
    Button tempButton;
    std::vector<Button> buttons; // the container for the buttons.
    //User a temp button and push it into the container.
    tempButton.name = "Edit Title";
    tempButton.x = 960 - 230;
    tempButton.y = 200;
    tempButton.w = 200;
    tempButton.h = 50;
    buttons.push_back(tempButton);

    tempButton.name = "Edit Body";
    tempButton.y = 260;
    buttons.push_back(tempButton);

    tempButton.name = "Add Footer";
    tempButton.y = 320;
    buttons.push_back(tempButton);

    tempButton.name = "Submit Issue";
    tempButton.y = 380;
    buttons.push_back(tempButton);

    tempButton.name = "Cancel";
    tempButton.y = 440;
    buttons.push_back(tempButton);


    while(!done) {
        update_keys();

        if(up_released)   cursor_pos -= 1;
        if(down_released) cursor_pos += 1;

        if(cursor_pos < 0) cursor_pos = 4;
        if(cursor_pos > 4) cursor_pos = 0;

        if(circle_released){
            reset_keys();
            break;
        }

        if(cross_released) {
            std::string message;
            switch (cursor_pos) {
                case 0:
                    title = vita_keyboard_get((char*)"Enter title", (const char*)title.c_str(), 600, 0);
                    break;
                case 1:
                    body = vita_keyboard_get((char*)"Enter body", (const char*)body.c_str(), 600, 1);
                    break;
                case 2:
                    if(show_footer)
                        show_footer = false;
                    else
                        show_footer = true;
                    break;
                case 3:
                    message = "Issue will be submitted to RepoHub\nusing the name: ";
                    message += user_name;
                    message += "\nWould you like to continue?";
                    if(draw_yes_no_message(message)) {
                        if(show_footer) body += footer;
                        curl_post_issue("https://api.github.com/repos/robDevs/RepoHub/issues", title, body);
                        done = true;
                    }
                    break;
                case 4:
                    done = true;
                    break;
            }
        }

        if(show_footer)
            buttons[2].name = "Add Footer";
        else
            buttons[2].name = "Remove Footer";

        read_joy_sticks();

        if(body_pos < 143 && ly < 117) body_pos -= (ly-127)/10;
        if(body_pos + vita2d_font_text_height(font20, 20.0f, word_wrap(body, 40).c_str()) + 60 > 544 && ly > 137) body_pos += (127-ly)/10;

        vita2d_start_drawing();
        vita2d_clear_screen();

        //draw the content and the foooter.

        vita2d_font_draw_text(font20, 40, body_pos, RGBA8(0,0,0,255), 20.0f, word_wrap(body, 40).c_str());

        if(show_footer) {
            vita2d_draw_rectangle(40, body_pos + vita2d_font_text_height(font20, 20.0f, word_wrap(body, 40).c_str()), 960-260-80, 10, RGBA8(225,228,232,255));

            vita2d_font_draw_text(font20, 40, body_pos + vita2d_font_text_height(font20, 20.0f, word_wrap(body, 40).c_str()) + 40, RGBA8(0,0,0,255), 20.0f, "Submitted from RepoHub on PS Vita");
        }

        //draw the template containing a sub header area and buttons area.

        drawTemplate(SUB_HEADER_BUTTONS, "Submit issue to RepoHub on Github", buttons, cursor_pos);

        //sub header goes over template. may add to template.
        vita2d_font_draw_text(font40, 40, 103 - 9, RGBA8(0,0,0,255), 40.0f, title.c_str());

        vita2d_end_drawing();
        vita2d_common_dialog_update();
        vita2d_swap_buffers();
    }
}

void draw_empty_rectangle(int x, int y, int w, int h, unsigned int color) {
    vita2d_draw_line(x, y, x + w, y, color);
    vita2d_draw_line(x, y, x, y + h, color);
    vita2d_draw_line(x, y + h, x + w, y + h, color);
    vita2d_draw_line(x + w, y, x + w, y + h, color);
}
