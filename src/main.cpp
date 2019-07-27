#include "includes.h"
#include "images.h"
#include "vita-keyboard.h"
#include "curl-tools.h"
#include "json-tools.h"
#include "file-io.h"
#include "repo.h"
#include "user.h"

int main()
{

    netInit(); // init net and then http
	httpInit();

    int status = MAIN_VIEW;

	std::string inputText;
    std::string finalText;

	vita2d_init();
	vita2d_set_clear_color(RGBA8(255, 255, 255, 255));

    loadTextures();

    std::vector<std::string> userNames;

    /*if(file_exists("ux0:/data/repo-browser/users.txt")) {
        read_file_lines("ux0:/data/repo-browser/users.txt", &userNames);
    }
    else {
        read_file_lines("ux0:/data/repo-browser/users.txt", &userNames);
    }*/

    jansson_parse_followers_list(curl_get_string("https://api.github.com/users/robDevs/following"), &userNames);
    set_token();
    jannson_get_rate_limits(curl_get_string("https://api.github.com/rate_limit"), &core_max, &core_remain, &search_max, &search_remain);

    std::vector<User> users;

    for(size_t i = 0; i < userNames.size(); i++) {
        User newUser(userNames[i]);
        users.push_back(newUser);
    }

	while (status != QUIT_APP) {

        //inputText = vita_keyboard_get((char*)"Enter New Text:", (const char*)"", 600, 0);

        switch (status) {
            case MAIN_VIEW:
                draw_user_list(users, &status);
                break;
        }

	}

	/*
	 * vita2d_fini() waits until the GPU has finished rendering,
	 * then we can free the assets freely.
	 */

    httpTerm();
	netTerm();

	vita2d_fini();
    deleteTextures();

	sceKernelExitProcess(0);
	return 0;
}
