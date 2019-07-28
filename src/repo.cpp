#include "repo.h"
#include "json-tools.h"

Repo::Repo(std::string name, std::string description, std::string license, std::string owner, std::string created_at, std::string updated_at, std::string homePage, std::string language, bool fork, bool priv, int forks, int stargazers_count, int open_issues_count) {
    this->name = name;
    this->description = description;
    this->license = license;
    this->owner = owner;
    this->created_at = created_at;
    this->updated_at = updated_at;
    this->homePage = homePage;
    this->language = language;
    this->fork = fork;
    this->priv = priv;
    this->forks = forks;
    this->stargazers_count = stargazers_count;
    this->open_issues_count = open_issues_count;
}

void Repo::setName(std::string name) {

}

std::string Repo::getName() {
    return name;
}

void Repo::setOwner(std::string owner) {
    this->owner = owner;
}

void Repo::setLicense(std::string license) {
    this->license = license;
}

std::string Repo::getLicense() {
    return "";
}

void Repo::setFork(bool fork) {
    this->fork = fork;
}

bool Repo::getFork() {
    return false;
}

void Repo::setForks(int forks) {
    this->forks = forks;
}

int Repo::getForks() {
    return 1;
}

void Repo::drawListView(int x, int y, bool selected) {
    draw_repo_list_item(name, description, language, stargazers_count, forks, updated_at, x, y, selected);
}

void Repo::drawDetailsView(std::string header_start) {
    init_keys();

    int cursor_pos = 0;

    std::string header_string = header_start;
    header_string += "->";
    header_string += name;

    while(1) {
        update_keys();

        if(up_released)   cursor_pos -= 1;
        if(down_released) cursor_pos += 1;

        if(cursor_pos < 0) cursor_pos = 1;
        if(cursor_pos > 1) cursor_pos = 0;

        if (circle_released){
            reset_keys();
            break;
        }

        if(cross_released) {
            if(cursor_pos == 0) {
                std::string url = "https://api.github.com/repos/";
                url += owner;
                url += "/";
                url += name;
                url += "/releases";
                std::string release_list_string = curl_get_string(url);
                jannson_get_rate_limits(curl_get_string("https://api.github.com/rate_limit"), &core_max, &core_remain, &search_max, &search_remain);

                check_core_rate_limit();

                jansson_parse_release_list(release_list_string, &releases);

                if(static_cast<int>(releases.size()) > 0) {
                    drawReleases(header_string);
                }
            }
            if(cursor_pos == 1)
                break;
        }

        vita2d_start_drawing();
        vita2d_clear_screen();

        draw_header(header_string);

        vita2d_font_draw_textf(font40, 40, 95, RGBA8(0,0,0,255), 40.0f, "%s/%s", owner.c_str(), name.c_str());
        vita2d_font_draw_textf(font20, 960 - vita2d_font_text_width(font20, 20.0f, license.c_str()) - 5, 124, RGBA8(0,0,0,255), 20.0f, "%s", license.c_str());
        vita2d_draw_line(0, 103, 960, 103, RGBA8(150,150,150,150));

        vita2d_font_draw_textf(font20, 40, 120, RGBA8(0,0,0,255), 20.0f, "Description:\n%s", word_wrap(description, 50).c_str());
        //vita2d_font_draw_textf(font20, 10, 100, RGBA8(0,0,0,255), 20.0f, "Created at: %s", created_at.c_str());
        //vita2d_font_draw_textf(font20, 10, 120, RGBA8(0,0,0,255), 20.0f, "Updated at: %s", updated_at.c_str());
        //vita2d_font_draw_textf(font20, 10, 140, RGBA8(0,0,0,255), 20.0f, "Homepage: %s", homePage.c_str());
        //vita2d_font_draw_textf(font20, 10, 160, RGBA8(0,0,0,255), 20.0f, "Language: %s", language.c_str());
        //if(fork)
            //vita2d_font_draw_textf(font20, 10, 180, RGBA8(0,0,0,255), 20.0f, "Fork: yes");
        //else
        //    vita2d_font_draw_textf(font20, 10, 180, RGBA8(0,0,0,255), 20.0f, "Fork: no");
        //vita2d_font_draw_textf(font20, 10, 200, RGBA8(0,0,0,255), 20.0f, "Forks: %d", forks);
        //vita2d_font_draw_textf(font20, 10, 220, RGBA8(0,0,0,255), 20.0f, "Stars: %d", stargazers_count);
        //vita2d_font_draw_textf(font20, 10, 240, RGBA8(0,0,0,255), 20.0f, "Open Issues: %d", open_issues_count);
        draw_button("View Releases", 960 - 250, 200, 200, 50, cursor_pos == 0);
        draw_button("Back", 960 - 250, 260, 200, 50, cursor_pos == 1);

        vita2d_end_drawing();
        vita2d_common_dialog_update();
        vita2d_swap_buffers();
    }
}

void Repo::drawReleases(std::string header_start) {
    int cursor_pos = 0;
    int y_offset = 103;
    int list_size = static_cast<int> (releases.size());

    float menuBarH = pow(544-103,2)/(list_size*100);

    std::string header_string = header_start;
    header_string += "->releases";

    init_keys();

    while(1) {
        update_keys();
        if (down_released) {
            if(cursor_pos < list_size - 1) cursor_pos += 1;
            else cursor_pos = 0;
        }

        if (up_released) {
            if(cursor_pos > 0) cursor_pos -= 1;
            else cursor_pos = list_size - 1;
        }

        if (circle_released){
            break;
        }

        if(y_offset + (cursor_pos*100) > 544 - 100) {
            y_offset -= 100;
        }

        if(y_offset + (cursor_pos*100) < 103) {
            y_offset += 100;
        }

        if(cross_released) {
            draw_release_details(releases[cursor_pos], header_string);
        }

        vita2d_start_drawing();
        vita2d_clear_screen();

        for(int i = 0; i < list_size; i++) {
            if(!releases[i].name.empty())
                draw_list_item(releases[i].name,"", "", y_offset + i*100, cursor_pos == i);
            else
                draw_list_item(releases[i].tag_name,"", "", y_offset + i*100, cursor_pos == i);
        }

        vita2d_draw_rectangle(0, 44, 960, 103-44, RGBA8(255,255,255,255));
        vita2d_font_draw_textf(font40, 40, 95, RGBA8(0,0,0,255), 40.0f, "%s/%s", owner.c_str(), name.c_str());
        vita2d_draw_line(0, 103, 960, 103, RGBA8(150,150,150,150));

        vita2d_draw_rectangle(960 - 10, 103 - y_offset*(menuBarH/(544-103)), 5, menuBarH, RGBA8(36,41,46,255));

        draw_header(header_string);

        vita2d_end_drawing();
        vita2d_common_dialog_update();
        vita2d_swap_buffers();
    }
}
