#ifndef _IMAGES_
#define _IMAGES_

#include "includes.h"

extern vita2d_texture *userBg;

extern vita2d_font *font20;
extern vita2d_font *font40;

extern void loadTextures();
extern void deleteTextures();

extern std::string word_wrap(std::string text, unsigned per_line);

#endif
