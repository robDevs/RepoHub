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

    std::vector<User> users;

    if(!file_exists("ux0:data/RepoHub/user.txt")) {
        sceIoMkdir("ux0:data/RepoHub", 0777);
        sceIoMkdir("ux0:data/RepoHub/Downloads", 0777);
    }
    else {
        get_user_name();
        set_user_list(&users);
    }

    if(file_exists("ux0:data/RepoHub/token.txt")) {
        get_token();
    }

    //clear the screen a few times?
    for(int i = 0; i < 4; i++) {
        vita2d_start_drawing();
        vita2d_clear_screen();
        vita2d_end_drawing();
        vita2d_common_dialog_update();
        vita2d_swap_buffers();
    }


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
