#include "draw.h"
#include <psp2/kernel/processmgr.h>
#include <psp2/rtc.h>

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
    int time_minute = sceRtcGetMinute(&time);
    time_string += std::to_string(time_hour);
    time_string += ":";
    if(time_minute < 10) time_string += "0";
    time_string += std::to_string(time_minute);
    time_string += am_pm;
    vita2d_draw_rectangle(0, 0, 960, 44, RGBA8(36,41,46,255));
    vita2d_font_draw_textf(font20, 20, 22+(vita2d_font_text_height(font20, 20.0f, header.c_str()) / 4), RGBA8(255,255,255,255), 20.0f, "%s", header.c_str());

    vita2d_font_draw_textf(font20, 960-(vita2d_font_text_width(font20, 20.0f, time_string.c_str())) - 20, 22+(vita2d_font_text_height(font20, 20.0f, header.c_str()) / 4), RGBA8(255,255,255,255), 20.0f, "%s", time_string.c_str());
}

void draw_button(std::string name, int x, int y, int w, int h, bool selected) {
    if(selected) {
        vita2d_draw_rectangle(x-2, y-2, w + 4, h+4, RGBA8(36,41,46,255));
        vita2d_draw_rectangle(x, y, w, h, RGBA8(255,255,255, 255));
        vita2d_font_draw_textf(font20, x+w/2 - (vita2d_font_text_width(font20, 20.0f, name.c_str()) / 2), y + 25+vita2d_font_text_height(font20, 20.0f, name.c_str()) / 4, RGBA8(0,0,0,255), 20.0f, "%s", name.c_str());
    }
    else {
        vita2d_draw_rectangle(x-1, y-1, w + 2, h + 2, RGBA8(150,150,150,150));
        vita2d_draw_rectangle(x, y, w, h, RGBA8(255,255,255, 255));
        vita2d_font_draw_textf(font20, x+w/2 - (vita2d_font_text_width(font20, 20.0f, name.c_str()) / 2), y + 25+vita2d_font_text_height(font20, 20.0f, name.c_str()) / 4, RGBA8(150,150,150,150), 20.0f, "%s", name.c_str());
    }
}

void draw_list_item(std::string name, std::string body, std::string language, int y, bool selected) {
    vita2d_draw_rectangle(0, y, 960, 100, RGBA8(255,255,255,255));
    vita2d_font_draw_textf(font40, 40, y+50+(vita2d_font_text_height(font40, 40.0f, name.c_str()) / 4), RGBA8(3,102,214,255), 40.0f, "%s", name.c_str());
    vita2d_font_draw_textf(font15, 50, y+50+(vita2d_font_text_height(font40, 40.0f, name.c_str())), RGBA8(88, 96, 105,255), 15.0f, "%s", body.c_str());

    if(selected) {
        vita2d_draw_line(0, y, 960, y, RGBA8(36,41,46,255));
        vita2d_draw_line(0, y+1, 960, y+1, RGBA8(36,41,46,255));
        vita2d_draw_line(0, y + 100, 960, y + 100, RGBA8(36,41,46,255));
        vita2d_draw_line(0, y + 100 - 1, 960, y + 100 - 1, RGBA8(36,41,46,255));
    }
    else {
        vita2d_draw_line(0, y, 960, y, RGBA8(150,150,150,150));
        vita2d_draw_line(0, y + 100, 960, y + 100, RGBA8(150,150,150,150));
    }
}

void draw_repo_list_item(std::string name, std::string body, std::string language, int stars, int forks, std::string updated_at, int x, int y, bool selected) {
    if(body.size() > 60) {
        body.resize(60);
        body += "...";
    }

    if(updated_at.size() > 10)
        updated_at.resize(10);

    int lang_circle_x = x+20+8;
    std::string stars_final = std::to_string(stars);
    std::string forks_final = std::to_string(forks);
    int language_x = lang_circle_x + 10;
    int star_icon_x = language_x + vita2d_font_text_width(font15, 15.0f, language.c_str()) + 5;
    int stars_x = star_icon_x + 15;
    int forks_icon = stars_x + vita2d_font_text_width(font15, 15.0f, stars_final.c_str()) + 5;
    int forks_x = forks_icon + 15;
    if(selected) {
        vita2d_draw_rectangle(x-2, y+3, 424, 144, RGBA8(255,255,255,255));
        vita2d_draw_rectangle(x-2, y+40-2, 424, 104, RGBA8(36,41,46,255));
        vita2d_draw_rectangle(x, y+40, 420, 100, RGBA8(255,255,255,255));
    }
    else {
        vita2d_draw_rectangle(x, y + 3, 424, 144, RGBA8(255,255,255,255));
        vita2d_draw_rectangle(x-2, y+40-2, 424, 104, RGBA8(150,150,150,150));
        vita2d_draw_rectangle(x, y+40, 420, 100, RGBA8(255,255,255,255));
    }
    vita2d_font_draw_textf(font20, x+20, y+40+25+(vita2d_font_text_height(font20, 20.0f, name.c_str()) / 4), RGBA8(3,102,214,255), 20.0f, "%s", name.c_str());
    vita2d_font_draw_textf(font15, x+20, y+40+25+(vita2d_font_text_height(font20, 20.0f, name.c_str())) + 5, RGBA8(88, 96, 105,255), 15.0f, "%s", word_wrap(body, 40).c_str());

    vita2d_draw_fill_circle(lang_circle_x, y+40+100-15, 7, get_lang_color(language));
    vita2d_font_draw_textf(font15, language_x, y+40+100-10, RGBA8(88, 96, 105,255), 15.0f, language.c_str());
    vita2d_font_draw_textf(font15, stars_x, y+40+100-10, RGBA8(88, 96, 105,255), 15.0f, stars_final.c_str());
    vita2d_font_draw_textf(font15, forks_x, y+40+100-10, RGBA8(88, 96, 105,255), 15.0f, forks_final.c_str());
    vita2d_draw_texture_scale(star, star_icon_x, y+40+100-23, 0.2188, 0.2188);
    vita2d_draw_texture_scale(fork_tex, forks_icon, y+40+100-23, 0.2188, 0.2188);

    std::string updated_at_final = "Updated at: ";
    updated_at_final += updated_at;
    vita2d_font_draw_text(font15, x + 420 - vita2d_font_text_width(font15, 15.0f, updated_at_final.c_str()) - 20, y+40+100-10, RGBA8(88, 96, 105,255), 15.0f, updated_at_final.c_str());
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
        vita2d_draw_line(0, y, 960, y, RGBA8(150,150,150,150));
        vita2d_draw_line(0, y + 100, 960, y + 100, RGBA8(150,150,150,150));
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

        if(left_released) cursor_pos -= 1;
        if(right_released) cursor_pos += 1;

        if(cursor_pos < 0) cursor_pos = 1;
        if(cursor_pos > 1) cursor_pos = 0;

        vita2d_start_drawing();

        vita2d_draw_rectangle(960 / 2 - 200, 544 / 2 - 100, 400, 200, RGBA8(36,41,46,255));

        vita2d_font_draw_text(font20, (960/2)-(vita2d_font_text_width(font20, 20.0f, message.c_str()) / 2), 544 / 2 - 40, RGBA8(255,255,255,255), 20.0f, message.c_str());

        draw_button("Yes", 960 / 2 - 100, 544/2 + 25, 50, 50, cursor_pos == 0);
        draw_button("No", 960 / 2 + 50, 544/2 + 25, 50, 50, cursor_pos == 1);

        vita2d_end_drawing();
        vita2d_common_dialog_update();
		vita2d_swap_buffers();
    }
    return return_val;
}
void draw_alert_message(std::string message) {
    init_keys();

    bool done = false;

    while(!done) {
        update_keys();
        if(cross_released)
            done = true;


        vita2d_start_drawing();

        vita2d_draw_rectangle(960 / 2 - 200, 544 / 2 - 100, 400, 200, RGBA8(36,41,46,255));

        vita2d_font_draw_text(font20, 960/2 - vita2d_font_text_width(font20, 20.0f, message.c_str()) / 2, 544 / 2 - 40, RGBA8(255,255,255,255), 20.0f, message.c_str());

        draw_button("Okay", 960 / 2 - 75 / 2, 544/2 + 25, 75, 50, true);

        vita2d_end_drawing();
        vita2d_common_dialog_update();
		vita2d_swap_buffers();
    }
}
