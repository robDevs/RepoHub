#include "release.h"
#include "draw.h"

#include "curl-tools.h"

#include <sstream>

void draw_release_details(Release release, std::string header_start) {
    init_keys();

    std::vector<std::string> body_vec;
    std::string temp = release.body;
    std::istringstream body_stream (temp);
    temp = "";
    while(std::getline(body_stream, temp)) {
        body_vec.push_back(temp);
    }

    int body_height = get_string_vector_height(body_vec, 80);

    int list_size = static_cast<int> (release.assets.size());
    int cursor_pos = 0;
    int y_offset = 103;

    float menuBarH = pow(420,2)/(list_size*100);

    int body_string_x = 40;
    int body_string_y = 145;

    std::string header_string = header_start;
    header_string += "/";
    if(!release.name.empty()) header_string += release.name;
    else header_string += release.tag_name;

    int status = 0;

    while(1) {
        update_keys();
        if(cross_released && list_size > 0 && status == 0) {

            if(release.assets[cursor_pos].size < get_space_avail()) {
                std::string message_string = "Download ";
                message_string += release.assets[cursor_pos].name;
                message_string += "\nto 'ux0:data/RepoHub/Downloads/?'";
                if(draw_yes_no_message(message_string)) {
                    std::string filePath = "ux0:data/RepoHub/Downloads/";
                    filePath += release.assets[cursor_pos].name;
                    curl_download_file(release.assets[cursor_pos].url, filePath);
                }
            }
            else {
                std::string message_string = "Not enough Space!";
                message_string += "\nFree space: ";
                message_string += std::to_string(byte_to_mb(get_space_avail()));
                message_string.resize(message_string.size() - 3);
                message_string += " MB";
                draw_alert_message(message_string);
            }
        }

        if(circle_released)
            break;

        while(y_offset + (cursor_pos*100) > 544 - 100) {
            y_offset -= 100;
        }

        while(y_offset + (cursor_pos*100) < 103) {
            y_offset += 100;
        }

        if(left_released) status -= 1;
        if(right_released) status += 1;
        if(status < 0) status = 1;
        if(status > 1) status = 0;

        if(up_released && status == 0) cursor_pos -= 1;
        if(down_released && status == 0) cursor_pos += 1;
        if(cursor_pos < 0) cursor_pos = list_size -1;
        if(cursor_pos > list_size -1) cursor_pos = 0;

        read_joy_sticks();

        if(status == 1) {
            if(body_string_y < 145 && ly < 127 - 20) body_string_y += (127-ly)/10;
            if(body_string_y + body_height > 544 - 44 && ly > 127 + 20) body_string_y -= (ly-127)/10;
        }

        vita2d_start_drawing();
        vita2d_clear_screen();

        if(status == 0) {
            //file list
            for(int i = 0; i < list_size; i++) {
                draw_list_item_small(release.assets[i].name, y_offset+i*100, cursor_pos == i);
            }

            //sub-header bg.
            vita2d_draw_rectangle(0, 44, 960, 103-44, RGBA8(255,255,255,255));
            vita2d_draw_line(0, 103, 960, 103, RGBA8(150,150,150,200));
            vita2d_font_draw_text(font40, 40, (45+40)+10, RGBA8(0,0,0,255), 40.0f, "Files");
            vita2d_font_draw_text(font40, 960-560, (45+40)+10, RGBA8(150,150,150,200), 40.0f, "Notes");

            if(menuBarH < 420)
                vita2d_draw_rectangle(960 - 15, 103 - (y_offset-100)*(menuBarH/(420)), 15, menuBarH, RGBA8(167,167,167,255));
        }
        else {
            //body and body bg
            //vita2d_font_draw_text(font15, body_string_x, body_string_y, RGBA8(0,0,0,255), 15.0f, final_body_string.c_str());

            draw_string_vector(body_vec, body_string_x, body_string_y, 80);

            //sub-header bg.
            vita2d_draw_rectangle(0, 44, 960, 80, RGBA8(255,255,255,255));
            vita2d_draw_line(0, 44+80, 960, 45+80, RGBA8(150,150,150,200));
            vita2d_font_draw_text(font40, 40, (45+40)+10, RGBA8(150,150,150,200), 40.0f, "Files");
            vita2d_font_draw_text(font40, 960-560, (45+40)+10, RGBA8(0,0,0,255), 40.0f, "Notes");
        }

        draw_header(header_string);

        vita2d_end_drawing();
        vita2d_common_dialog_update();
        vita2d_swap_buffers();
    }
}
