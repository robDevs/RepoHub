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

	//vita2d_init();
    vita2d_init_advanced_with_msaa( (1 * 1024 * 1024), SCE_GXM_MULTISAMPLE_4X );
	vita2d_set_clear_color(RGBA8(255, 255, 255, 255));

    loadTextures();

    if(!file_exists("ux0:data/repo-browser/user.txt")) {
        sceIoMkdir("ux0:data/repo-browser", 0777);
        sceIoMkdir("ux0:data/repo-browser/Downloads", 0777);
        if(draw_yes_no_message("A user name is required to get list of\nusers you have followed.\nEnter user name now?")) {
            set_user_name();
        }
        if(draw_yes_no_message("A user access token can be used for more api requests.\nYou can create one at \"https://github.com/settings/tokens\"\nEnter token now?")) {
            set_token();
        }
    }
    else {
        get_user_name();
    }

    if(file_exists("ux0:data/repo-browser/token.txt")) {
        get_token();
    }
    else if(draw_yes_no_message("A user access token can be used for more api requests.\nYou can create one at \"https://github.com/settings/tokens\"\nEnter token now?")) {
        set_token();
    }

    std::vector<User> users;

    set_user_list(&users);


	while (status != QUIT_APP) {

        //inputText = vita_keyboard_get((char*)"Enter New Text:", (const char*)"", 600, 0);

        switch (status) {
            case MAIN_VIEW:
                draw_user_list(&users, &status);
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
