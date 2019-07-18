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

#define MAIN_VIEW       0//view the main list of users
#define SEARCH_VIEW     1//search the api
#define RESULT_VIEW     2//results of the search
#define USER_VIEW       3//view a selected user
#define REPO_VIEW       4//view a selected repo
#define RELEASE_VIEW    5//view a selected release from a repo
#define QUIT_APP        6//quit the app

#endif
