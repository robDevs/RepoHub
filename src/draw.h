#ifndef _DRAW_
#define _DRAW_
#include "includes.h"
#include "images.h"
#include "controls.h"

extern void draw_header(std::string header);
extern void draw_button(std::string name, int x, int y, bool selected);
extern void draw_list_item(std::string name, int y, bool selected);

#endif
