#include "draw_templates.h"

extern void drawTemplate(int type, std::string header, std::vector<Button> buttons, int cursor_pos) {
    switch (type) {
        case SUB_HEADER_BUTTONS:
            subHeaderButtons(header, buttons, cursor_pos);
            break;
        default:
            break;
    }
}

extern void subHeaderButtons(std::string header, std::vector<Button> buttons, int cursor_pos) {
    //right hand menu bar
    vita2d_draw_rectangle(960 - 260, 103, 260, 544 - 103, RGBA8(255,255,255,255));
    vita2d_draw_line(960-260, 104, 960-260, 544, RGBA8(150,150,150,200));

    //sub header area
    vita2d_draw_rectangle(0, 44, 960, 103-44, RGBA8(255,255,255,255));
    vita2d_draw_line(0, 103, 960, 103, RGBA8(150,150,150,200));

    int num_buttons = static_cast<int>(buttons.size());

    for(int i = 0; i < num_buttons; i++) {
        draw_button(buttons[i], cursor_pos == i);
    }

    //draw the header
    draw_header(header);
}
