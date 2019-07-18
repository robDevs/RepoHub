#ifndef _FILE_IO_
#define _FILE_IO_

#include "includes.h"

extern std::string token;
extern bool have_token;

extern bool file_exists(std::string filePath);
extern void read_file_lines(std::string filePath, std::vector<std::string> *list);
extern void write_token(std::string token);
extern void get_token();

extern std::string encrypt_string(std::string msg, std::string const& key);
extern std::string decrypt_string(std::string msg, std::string const& key);

#endif
