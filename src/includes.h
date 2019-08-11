#ifndef _INCLUDES_
#define _INCLUDES_

#include <string.h>
#include <stdbool.h>
#include <psp2/apputil.h>
#include <psp2/display.h>
#include <psp2/kernel/processmgr.h>
#include <psp2/ime_dialog.h>
#include <psp2/message_dialog.h>
#include <psp2/ctrl.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vita2d.h>

#include <psp2/io/stat.h>
#include <psp2/sysmodule.h>
#include <psp2/net/net.h>
#include <psp2/net/netctl.h>
#include <psp2/net/http.h>

#include <psp2/io/fcntl.h>
#include <math.h>
#include <stdio.h>
#include <malloc.h>
#include <curl/curl.h>
#include <cmath>

#include <vector>

#include <jansson.h>

#define TAG                 0.00 // the current version. Used by updater.

#define MAIN_VIEW           0//view the main list of users
#define UPDATE_USERNAME     1//update the username
#define UPDATE_TOKEN        2//update the token
#define SUBMIT_ISSUE        3
#define RETURN_STATE        4//return state
#define REPO_VIEW           5//view a selected repo
#define RELEASE_VIEW        6//view a selected release from a repo
#define STARRED_VIEW        7
#define QUIT_APP            8//quit the app

extern std::string token;
extern bool have_token;

extern bool authed;

extern int core_max;
extern int core_remain;
extern int search_max;
extern int search_remain;

extern unsigned int get_lang_color(std::string language);

extern float byte_to_kb(uint64_t bytes);
extern float byte_to_mb(uint64_t bytes);
extern float byte_to_gb(uint64_t bytes);

extern std::string strip_html_tags(std::string msg);

extern std::string user_name;
extern int following_count;

extern void escape_new_line(std::string *string);

extern void draw_string_vector(std::vector<std::string> msg,int x, int y, int per_line);
extern int get_string_vector_height(std::vector<std::string> msg, int per_line);

extern void check_update(); //updater

#endif
