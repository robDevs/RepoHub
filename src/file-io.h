#ifndef _FILE_IO_
#define _FILE_IO_

#include "includes.h"

extern std::string token;
extern bool have_token;

extern bool file_exists(std::string filePath);
extern void read_file_lines(std::string filePath, std::vector<std::string> *list);
extern void get_token();


#endif
