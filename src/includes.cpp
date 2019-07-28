#include "includes.h"

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
