#ifndef INCLUDE_utils_file_h__
#define INCLUDE_utils_file_h__
#include "../common.h"

bool is_file(const char* path);
bool is_dir(const char* path);
bool is_link(const char* path);
bool file_exists(const char* path);
bool is_executable(const char* path);
char *get_program_path();

#endif