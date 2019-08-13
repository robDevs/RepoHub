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
    this->readme = "";
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
    draw_repo_list_item(name, description, language, stargazers_count, forks, updated_at, fork, x, y, selected);
}

void Repo::drawDetailsView(std::string header_start) {
    int starred = false;
    if(authed) {
        std::string url = "https://api.github.com/user/starred/";
        url += owner;
        url += "/";
        url += name;
        starred = curl_check_star(url);
    }

    init_keys();

    if(readme_vec.empty())
        setReadme();

    //strip_new_line(readme);

    int cursor_pos = 0;

    std::string final_desc = "Description:\n";
    final_desc += word_wrap(description, 50);

    std::string final_readme = "Readme:\n";
    final_readme += readme;

    int desc_y = 120;
    int readme_y = desc_y + vita2d_font_text_height(font20, 20.0f, final_desc.c_str()) + 20;

    int desc_readme_height = vita2d_font_text_height(font20, 20.0f, final_desc.c_str()) + get_readme_vec_height() + 40;

    std::string header_string = header_start;
    header_string += "/";
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
                if(releases.size() < 1) {
                    std::string url = "https://api.github.com/repos/";
                    url += owner;
                    url += "/";
                    url += name;
                    url += "/releases";
                    std::string release_list_string = curl_get_string(url);
                    jannson_get_rate_limits(curl_get_string("https://api.github.com/rate_limit"), &core_max, &core_remain, &search_max, &search_remain);

                    check_core_rate_limit();

                    jansson_parse_release_list(release_list_string, &releases);
                }

                if(static_cast<int>(releases.size()) > 0) {
                    drawReleases(header_string);
                }
                else {
                    draw_alert_message("No releases available!");
                }
            }
            if(cursor_pos == 1) {
                std::string url = "https://api.github.com/user/starred/";
                url += owner;
                url += "/";
                url += name;
                if(starred) {
                    curl_post_star(url, true);
                    starred = false;
                }
                else {
                    curl_post_star(url, false);
                    starred = true;
                }
            }
        }

        if(!authed) {
            cursor_pos = 0;
        }

        read_joy_sticks();

        if(desc_y + desc_readme_height > 504 && ly > 147) desc_y += (127-ly)/10;
        if(desc_y < 120 && ly < 107) desc_y -= (ly-127)/10;

        readme_y = desc_y + vita2d_font_text_height(font20, 20.0f, final_desc.c_str()) + 20;


        vita2d_start_drawing();
        vita2d_clear_screen();

        vita2d_font_draw_text(font20, 40, desc_y, RGBA8(0,0,0,255), 20.0f, final_desc.c_str());
        //vita2d_font_draw_text(font20, 40, readme_y, RGBA8(0,0,0,255), 20.0f, final_readme.c_str());
        draw_readme_vec(readme_y);
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

        vita2d_draw_rectangle(960 - 260, 103, 260, 544 - 103, RGBA8(255,255,255,255));
        vita2d_draw_line(960-260, 104, 960-260, 544, RGBA8(150,150,150,200));
        draw_button("View Releases", 960 - 230, 200, 200, 50, cursor_pos == 0);
        if(authed) {
            if(starred)
                draw_button("Unstar", 960 - 230, 260, 200, 50, cursor_pos == 1);
            else
                draw_button("Star", 960 - 230, 260, 200, 50, cursor_pos == 1);
        }

        vita2d_font_draw_textf(font15, 960 - vita2d_font_text_width(font15, 15.0f, license.c_str()) - 5, 120, RGBA8(0,0,0,255), 15.0f, "%s", license.c_str());

        vita2d_draw_rectangle(0, 44, 960, 103-44, RGBA8(255,255,255,255));
        vita2d_font_draw_textf(font40, 40, 95, RGBA8(0,0,0,255), 40.0f, "%s/%s", owner.c_str(), name.c_str());
        vita2d_draw_line(0, 103, 960, 103, RGBA8(150,150,150,200));

        draw_header(header_string);

        vita2d_end_drawing();
        vita2d_common_dialog_update();
        vita2d_swap_buffers();
    }
}

void Repo::drawReleases(std::string header_start) {
    int cursor_pos = 0;
    int y_offset = 103;
    int list_size = static_cast<int> (releases.size());

    float menuBarH = pow(441,2)/(list_size*100);

    std::string header_string = header_start;
    header_string += "/releases";

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

        while(y_offset + (cursor_pos*100) > 544 - 100) {
            y_offset -= 100;
        }

        while(y_offset + (cursor_pos*100) < 103) {
            y_offset += 100;
        }

        if(cross_released) {
            draw_release_details(releases[cursor_pos], header_string);
        }

        vita2d_start_drawing();
        vita2d_clear_screen();

        for(int i = 0; i < list_size; i++) {
            draw_release_list_item(releases[i].name, releases[i].tag_name, releases[i].author, releases[i].published_at, 40, y_offset + i*100, cursor_pos == i);
        }

        vita2d_draw_rectangle(0, 44, 960, 103-44, RGBA8(255,255,255,255));
        vita2d_font_draw_textf(font40, 40, 95, RGBA8(0,0,0,255), 40.0f, "%s/%s", owner.c_str(), name.c_str());
        vita2d_draw_line(0, 103, 960, 103, RGBA8(150,150,150,200));

        if(menuBarH < 420)
            vita2d_draw_rectangle(960 - 15, 103 - (y_offset-100)*(menuBarH/(420)), 15, menuBarH, RGBA8(167,167,167,255));

        draw_header(header_string);

        vita2d_end_drawing();
        vita2d_common_dialog_update();
        vita2d_swap_buffers();
    }
}

void Repo::setReadme() {
    std::string readme_url = "https://api.github.com/repos/";
    readme_url += owner;
    readme_url += "/";
    readme_url += name;
    readme_url += "/readme";
    jansson_get_readme(curl_get_string(readme_url), &readme_vec);
    //readme = strip_html_tags(readme);
    //strip_carriage_return(readme);

    for(size_t i = 0; i < readme_vec.size(); i++) {
        readme_vec[i] = strip_html_tags(readme_vec[i]);
        strip_carriage_return(readme_vec[i]);
        //strip_new_line(readme_vec[i]);
    }
}

void Repo::draw_readme_vec(int y) {
    int offset = 0;

    vita2d_font_draw_text(font20, 40, y + offset, RGBA8(0,0,0,255), 20.0f, "Readme:\n");
    offset += vita2d_font_text_height(font20, 20.0f, "Readme:\n") + 3;

    for(size_t i = 0; i < readme_vec.size(); i++) {
        vita2d_font_draw_text(font20, 40, y + offset, RGBA8(0,0,0,255), 20.0f, word_wrap(readme_vec[i], 50).c_str());

        offset += vita2d_font_text_height(font20, 20.0f, word_wrap(readme_vec[i], 50).c_str()) + 3;
    }
}

int Repo::get_readme_vec_height() {
    int h = 0;
    h += vita2d_font_text_height(font20, 20.0f, "Readme:\n") + 3;

    for(size_t i = 0; i < readme_vec.size(); i++) {
        h += vita2d_font_text_height(font20, 20.0f, word_wrap(readme_vec[i], 50).c_str()) + 3;
    }

    return h;
}
