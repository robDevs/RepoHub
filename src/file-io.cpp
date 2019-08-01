#include "file-io.h"
#include <iostream>
#include <fstream>
#include <psp2/io/devctl.h>
#include <psp2/appmgr.h>
#include "key.h"
#include "vita-keyboard.h"
#include "draw.h"



std::string token = "";
bool have_token = false;

bool file_exists(std::string filePath) {
    if (FILE *file = fopen(filePath.c_str(), "r")) {
        fclose(file);
        return true;
    } else {
        return false;
    }
}

void read_file_lines(std::string filePath, std::vector<std::string> *list) {
    std::string line;
    std::ifstream file (filePath.c_str());
    if (file.is_open()) {
        while(getline(file,line)) {
            list->push_back(line);
        }
    }
    file.close();
}


void get_token() {
    token = decrypt(read_file("ux0:data/RepoHub/token.txt"), SUPER_SECRET_KEY);
    if(!token.empty()) {
        have_token = true;
    }
}

void set_token() {
    if(draw_yes_no_message("A personal access token can be used for more api requests.\nYou can create one at \"https://github.com/settings/tokens\"\nEnter token now?"))
    {
        std::string temp_token = vita_keyboard_get((char*)"Enter token", (const char*)"", 600, 0);

        if(!temp_token.empty()) {
            write_to_file(encrypt(temp_token, SUPER_SECRET_KEY), "ux0:data/RepoHub/token.txt");
            get_token();
        }
    }
}

void set_user_name() {
    if(draw_yes_no_message("A user name is required to get the list of\nusers you have followed.\nEnter user name now?"))
    {
        user_name = vita_keyboard_get((char*)"Enter username:", (const char*)"", 600, 0);
        write_to_file(user_name, "ux0:data/RepoHub/user.txt");
    }
}

void get_user_name() {
    user_name = read_file("ux0:data/RepoHub/user.txt");
}

void write_to_file(std::string message, std::string path) {
    std::ofstream file;
    file.open (path.c_str());
    //file << message.c_str();
    for(size_t i = 0; i < message.size(); i++) {
        file.put(message[i]);
    }
    file.close();
}

//not working correctly. It reads the size of something?
uint64_t get_space_avail() {
    uint64_t max, free_space;
    max = 0;
    free_space = 0;

    sceAppMgrGetDevInfo("ux0:", &max, &free_space);

    return free_space;
}

std::string encrypt(std::string msg, std::string const& key)
{
    if(!key.size())
        return msg;

    for (std::string::size_type i = 0; i < msg.size(); ++i)
        msg[i] ^= key[i%key.size()];
    return msg;
}

std::string decrypt(std::string const& msg, std::string const& key)
{
    return encrypt(msg, key);
}

std::string read_file(std::string path) {
    char ch;
    std::fstream file(path.c_str(), std::fstream::in);
    std::string ret_string;
    while (file.get(ch)) {
        ret_string += ch;
    }
    return ret_string;
}
