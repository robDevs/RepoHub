#include "includes.h"
#include "draw.h"
#include "json-tools.h"
#include "curl-tools.h"

std::string token = "";
bool have_token = false;
bool authed = false;

int core_max = 0;
int core_remain = 0;
int search_max = 0;
int search_remain = 0;

unsigned int get_lang_color(std::string language) {
    if(language.compare("JavaScript") == 0)
        return RGBA8(241, 224, 90,255);
    if(language.compare("Java") == 0)
        return RGBA8(176, 114, 25,255);
    if(language.compare("Python") == 0)
        return RGBA8(53, 114, 165,255);
    if(language.compare("CSS") == 0)
        return RGBA8(86, 61, 124,255);
    if(language.compare("PHP") == 0)
        return RGBA8(79, 93, 149,255);
    if(language.compare("Ruby") == 0)
        return RGBA8(112, 21, 22,255);
    if(language.compare("C++") == 0)
        return RGBA8(243, 75, 125,255);
    if(language.compare("C") == 0)
        return RGBA8(85, 85, 85,255);
    if(language.compare("Shell") == 0)
        return RGBA8(137, 224, 81,255);
    if(language.compare("C#") == 0)
        return RGBA8(23, 134, 0,255);
    if(language.compare("Objective-C") == 0)
        return RGBA8(67, 142, 255,255);
    if(language.compare("R") == 0)
        return RGBA8(25, 140, 231,255);
    if(language.compare("Vim") == 0)
        return RGBA8(25, 159, 75,255);
    if(language.compare("Go") == 0)
        return RGBA8(0, 173, 216,255);
    if(language.compare("Perl") == 0)
        return RGBA8(2, 152, 195,255);
    if(language.compare("CoffeeScript") == 0)
        return RGBA8(36, 71, 118,255);
    if(language.compare("TeX") == 0)
        return RGBA8(61, 97, 23,255);
    if(language.compare("Swift") == 0)
        return RGBA8(255, 172, 69,255);
    if(language.compare("Scala") == 0)
        return RGBA8(194, 45, 64,255);
    if(language.compare("Common Lisp") == 0)
        return RGBA8(63, 182, 139,255);
    if(language.compare("Haskell") == 0)
        return RGBA8(94, 80, 134,255);
    if(language.compare("Lua") == 0)
        return RGBA8(0, 0, 128,255);
    if(language.compare("Clojure") == 0)
        return RGBA8(219, 88, 85,255);
    if(language.compare("MATLAB") == 0)
        return RGBA8(225, 103, 55,255);
    if(language.compare("Makefile") == 0)
        return RGBA8(66, 120, 25,255);
    if(language.compare("Groovy") == 0)
        return RGBA8(230, 159, 86,255);
    if(language.compare("Rust") == 0)
        return RGBA8(222, 165, 132,255);
    if(language.compare("PowerShell") == 0)
        return RGBA8(1, 36, 86,255);
    if(language.compare("HTML") == 0)
        return RGBA8(227, 76, 38,255);
    if(language.compare("TypeScript") == 0)
        return RGBA8(43, 116, 137,255);
    if(language.compare("OCaml") == 0)
        return RGBA8(59, 225, 51,255);
    if(language.compare("Pascal") == 0)
        return RGBA8(227, 241, 113,255);
    return RGBA8(36, 41, 46,255);
}

float byte_to_kb(uint64_t bytes) {
    return (float)bytes/1024;
}
float byte_to_mb(uint64_t bytes) {
    return (float)bytes/1024/1024;
}
float byte_to_gb(uint64_t bytes) {
    return (float)bytes/1024/1024/1024;
}

std::string strip_html_tags(std::string msg) {
    while (msg.find("<") != std::string::npos)
    {
        auto startpos = msg.find("<");
        auto endpos = msg.find(">") + 1;

        if (endpos != std::string::npos)
        {
            msg.erase(startpos, endpos - startpos);
        }
    }
    return msg;
}

std::string user_name = "";
int following_count = 0;

void escape_new_line(std::string *string) {
    while(string->find("\n") != std::string::npos) {
        int pos = string->find("\n");
        string->replace(pos,1,"\\n");;
    }
}

void draw_string_vector(std::vector<std::string> msg, int x, int y, int per_line) {
    int offset = 0;

    for(size_t i = 0; i < msg.size(); i++) {
        strip_carriage_return(msg[i]);
        vita2d_font_draw_text(font20, x, y + offset, RGBA8(0,0,0,255), 20.0f, word_wrap(msg[i], per_line).c_str());

        offset += vita2d_font_text_height(font20, 20.0f, word_wrap(msg[i], per_line).c_str()) + 3;
    }
}

int get_string_vector_height(std::vector<std::string> msg, int per_line) {
    int h = 0;

    for(size_t i = 0; i < msg.size(); i++) {
        h += vita2d_font_text_height(font20, 20.0f, word_wrap(msg[i], per_line).c_str()) + 3;
    }

    return h;
}

void check_update() {
    float latest_tag = 0.00;
    std::string url;
    jansson_get_tag_from_release(curl_get_string("https://api.github.com/repos/robDevs/RepoHub/releases/latest"), &latest_tag, &url);
    std::string tag_string = std::to_string(latest_tag);
    tag_string.resize(4);
    if(latest_tag > TAG) {
        std::string message = "New version available: v";
        message += tag_string;
        message += "\nWould you like to download the update?";

        if(draw_yes_no_message(message), "RepoHub.vpk") {
            std::string file_name = "ux0:data/RepoHub/Downloads/RepoHub_";
            file_name += tag_string;
            file_name += ".vpk";
            curl_download_file(url, file_name);
        }
    }
}
