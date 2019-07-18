#include "file-io.h"

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

void write_token(std::string token) {
    std::ofstream file ("ux0:/data/repo-browser/token.txt");
    if (file.is_open()) {
        file << encrypt_string(token, _SUPER_PRIVATE_KEY_).c_str();
        file.close();
    }
}

void get_token() {
    std::string line;
    std::ifstream file ("ux0:/data/repo-browser/token.txt");
    if (file.is_open()) {
        getline(file, line);
        token = decrypt_string(line, _SUPER_PRIVATE_KEY_);
        have_token = true;
    }
    file.close();
}

std::string encrypt_string(std::string msg, std::string const& key) {
    //make sure key is long engouh
    if(key.empty())
        return msg;

    for(size_t i = 0; i < msg.size(); ++i) {
        msg[i] ^= key[i%key.size()];
    }
    return msg;
}
std::string decrypt_string(std::string msg, std::string const& key) {
    return encrypt_string(msg, key);
}
