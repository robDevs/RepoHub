#ifndef _DRAW_
#define _DRAW_
#include "includes.h"
#include "images.h"
#include "controls.h"

extern void draw_header(std::string header);

extern void draw_button(std::string name, int x, int y, int w, int h, bool selected);

extern void draw_list_item(std::string name, int y, bool selected);
extern void draw_list_item_small(std::string name, int y, bool selected);

extern bool draw_yes_no_message(std::string message);
extern void draw_alert_message(std::string message);

#endif
