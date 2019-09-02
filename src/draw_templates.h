#ifndef _DRAW_TEMPATES_
#define _DRAW_TEMPATES_

#include "includes.h"
#include "draw.h"

#include <string>
#include <vector>

#define SUB_HEADER_BUTTONS      1
#define BUTTONS_ONLY            2

extern void drawTemplate(int type, std::string header, std::vector<Button> buttons, int cursor_pos);

extern void subHeaderButtons(std::string header, std::vector<Button> buttons, int cursor_pos);

extern void buttonsOnly(std::string header, std::vector<Button> buttons, int cursor_pos);

#endif
