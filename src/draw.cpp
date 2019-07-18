#include "draw.h"

void draw_header(std::string header) {
    vita2d_draw_rectangle(0, 0, 960, 44, RGBA8(36,41,46,255));
    vita2d_font_draw_textf(font20, 5, 22+(vita2d_font_text_height(font20, 20.0f, header.c_str()) / 2), RGBA8(255,255,255,255), 20.0f, "%s", header.c_str());
}

void draw_button(std::string name, int x, int y, bool selected) {
    if(selected) {
        vita2d_draw_rectangle(x-1, y-1, 212, 52, RGBA8(150,150,150,150));
        vita2d_draw_rectangle(x, y, 10, 50, RGBA8(255,0,0,255));
        vita2d_draw_rectangle(x + 10, y, 200, 50, RGBA8(255,255,255, 255));
        vita2d_font_draw_textf(font20, x + 15, y + 35, RGBA8(0,0,0,255), 20.0f, "%s", name.c_str());
    }
    else {
        vita2d_draw_rectangle(x-1, y-1, 202, 52, RGBA8(150,150,150,150));
        vita2d_draw_rectangle(x, y, 200, 50, RGBA8(255,255,255, 255));
        vita2d_font_draw_textf(font20, x + 5, y + 35, RGBA8(0,0,0,255), 20.0f, "%s", name.c_str());
    }
}
