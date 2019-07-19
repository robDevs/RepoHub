#include "file-io.h"
#include <iostream>
#include <fstream>
#include <psp2/io/devctl.h>
#include <psp2/appmgr.h>



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
    std::string line;
    std::ifstream file ("ux0:/data/repo-browser/token.txt");
    if (file.is_open()) {
        getline(file, line);
        token = line;
        have_token = true;
    }
    file.close();
}

void write_to_file(std::string message, std::string path) {
    std::ofstream file;
    file.open (path.c_str());
    file << message.c_str();
    file.close();
}

//not working correctly. It reads the size of something?
uint64_t get_space_avail() {
    uint64_t max, free;
    max = 0;
    free = 0;

    sceAppMgrGetDevInfo("ux0:", &max, &free);

    return free;
}
