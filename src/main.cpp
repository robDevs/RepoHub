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

	//vita2d_init();
    vita2d_init_advanced_with_msaa( (1 * 1024 * 1024), SCE_GXM_MULTISAMPLE_4X );
	vita2d_set_clear_color(RGBA8(255, 255, 255, 255));

    loadTextures();

    std::vector<std::string> userNames;
    std::vector<std::string> avatar_urls;
    std::string user_name = "";

    if(!file_exists("ux0:data/repo-browser/user.txt")) {
        sceIoMkdir("ux0:data/repo-browser", 0777);
        sceIoMkdir("ux0:data/repo-browser/Downloads", 0777);
        user_name = vita_keyboard_get((char*)"Enter username:", (const char*)"", 600, 0);
        write_to_file(user_name, "ux0:data/repo-browser/user.txt");
        set_token();
    }
    else {
        get_token();
        user_name = read_file("ux0:data/repo-browser/user.txt");
    }

    std::string url = "https://api.github.com/users/";
    url += user_name;
    url += "/following";

    jansson_parse_followers_list(curl_get_string(url), &userNames, &avatar_urls);
    jannson_get_rate_limits(curl_get_string("https://api.github.com/rate_limit"), &core_max, &core_remain, &search_max, &search_remain);

    std::vector<User> users;

    for(size_t i = 0; i < userNames.size(); i++) {
        User newUser(userNames[i], avatar_urls[i]);
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
    for(size_t i = 0; i < users.size(); i++){
        users[i].cleanUp();
    }

	sceKernelExitProcess(0);
	return 0;
}
