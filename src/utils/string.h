#ifndef INCLUDE_utils_string_h__
#define INCLUDE_utils_string_h__
#include "../common.h"
#include "list.h"

int strpos(const char *haystack, char *needle);

int strrpos(const char *haystack, char *needle);

char *substr(const char *str, int start, int length);

void trim(char *str);

void trim_chr(char *str, const char chr);

void trim_multi(char *str, const char *charlist, size_t charlist_size);

void strlwr(char *str);

void strupr(char *str);

int hex2dec(char chr);

int unescape(char *str);

list_t *explode(const char *delim, char *str);

char *implode(const char *delim, list_t *list);

#endif