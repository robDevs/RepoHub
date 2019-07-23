#ifndef _FILE_IO_
#define _FILE_IO_

#include "includes.h"
#include <psp2/io/stat.h>
#include <psp2/io/dirent.h>



extern std::string token;
extern bool have_token;

extern bool file_exists(std::string filePath);
extern void read_file_lines(std::string filePath, std::vector<std::string> *list);
extern void get_token();
extern void set_token();
extern void write_to_file(std::string message, std::string path);
extern uint64_t get_space_avail();
extern std::string encrypt(std::string msg, std::string const& key);
extern std::string decrypt(std::string const& msg, std::string const& key);
extern std::string read_file(std::string path);


#endif
