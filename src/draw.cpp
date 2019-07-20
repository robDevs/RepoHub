#include "draw.h"

void draw_header(std::string header) {
    vita2d_draw_rectangle(0, 0, 960, 44, RGBA8(36,41,46,255));
    vita2d_font_draw_textf(font20, 20, 22+(vita2d_font_text_height(font20, 20.0f, header.c_str()) / 4), RGBA8(255,255,255,255), 20.0f, "%s", header.c_str());
}

void draw_button(std::string name, int x, int y, int w, int h, bool selected) {
    if(selected) {
        vita2d_draw_rectangle(x-2, y-2, w + 4, h+4, RGBA8(36,41,46,255));
        vita2d_draw_rectangle(x, y, w, h, RGBA8(255,255,255, 255));
        vita2d_font_draw_textf(font20, x + 5, y + 25+vita2d_font_text_height(font20, 20.0f, name.c_str()) / 4, RGBA8(0,0,0,255), 20.0f, "%s", name.c_str());
    }
    else {
        vita2d_draw_rectangle(x-1, y-1, w + 2, h + 2, RGBA8(150,150,150,150));
        vita2d_draw_rectangle(x, y, w, h, RGBA8(255,255,255, 255));
        vita2d_font_draw_textf(font20, x + 5, y + 25+vita2d_font_text_height(font20, 20.0f, name.c_str()) / 4, RGBA8(0,0,0,255), 20.0f, "%s", name.c_str());
    }
}

void draw_list_item(std::string name, int y, bool selected) {
    vita2d_draw_rectangle(0, y, 960, 100, RGBA8(255,255,255,255));
    vita2d_font_draw_textf(font40, 40, y+50+(vita2d_font_text_height(font40, 40.0f, name.c_str()) / 4), RGBA8(0,0,0,255), 40.0f, "%s", name.c_str());
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
