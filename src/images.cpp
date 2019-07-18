#include "images.h"

vita2d_texture *userBg = NULL;

vita2d_font *font20;
vita2d_font *font40;

void loadTextures() {
    userBg = vita2d_load_PNG_file("app0:/images/user-background-list.png");
    font20 = vita2d_load_font_file("app0:/font.ttf");
    font40 = vita2d_load_font_file("app0:/font.ttf");
}

void deleteTextures() {
    vita2d_free_texture(userBg);

    vita2d_free_font(font20);
    vita2d_free_font(font40);
}

std::string word_wrap(std::string text, unsigned per_line) {
  //http://www.cplusplus.com/forum/beginner/132223/#msg714543
  unsigned line_begin = 0;
  while (line_begin < text.size()) {
    const unsigned ideal_end = line_begin + per_line ;
    unsigned line_end = (ideal_end <= text.size() ? ideal_end : text.size()-1);

    if (line_end == text.size() - 1) {
        ++line_end;
    } else if (std::isspace(text[line_end])) {
      text[line_end] = '\n';
      ++line_end;
    } else {   // backtrack
      unsigned end = line_end;
      while ( end > line_begin && !std::isspace(text[end])) {
        --end;
      }
      if (end != line_begin) {
        line_end = end;
        text[line_end++] = '\n';
      }
      else {
        text.insert(line_end++, 1, '\n');
      }
    }
    line_begin = line_end;
  }
  return text;
}
