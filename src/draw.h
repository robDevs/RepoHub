#ifndef _DRAW_
#define _DRAW_
#include "includes.h"
#include "images.h"
#include "controls.h"

struct Button{
    std::string name;
    int x;
    int y;
    int w;
    int h;
};

extern void draw_header(std::string header);

extern void draw_button(std::string name, int x, int y, int w, int h, bool selected);
extern void draw_button(Button button, bool selected);

extern void draw_list_item(std::string name, std::string body, std::string language, int x, int y, bool selected);
extern void draw_repo_list_item(std::string name, std::string body, std::string language, int stars, int forks, std::string update_at, bool fork, int x, int y, bool selected);
extern void draw_list_item_small(std::string name, int y, bool selected);

extern void draw_release_list_item(std::string name, std::string tag_name, std::string author, std::string published_at, int x, int y, bool selected);

extern bool draw_yes_no_message(std::string message);
extern void draw_alert_message(std::string message);
extern void draw_alert_message_time(std::string message, int time);

extern void draw_extract_progress(std::string path, std::string fileName, double fileProgress, double fileTotal, double zipProgress, double zipTotal);

extern int draw_star_menu();

extern void draw_issue_menu();

extern void draw_empty_rectangle(int x, int y, int w, int h, unsigned int color);

#endif
