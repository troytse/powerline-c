#ifndef INCLUDE_common_h__
#define INCLUDE_common_h__

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <string.h>
#include <regex.h>
#include <sys/types.h>
#include <sys/stat.h>

#ifdef DEBUG
#define debug(M, ...) fprintf(stderr, "%s:%d - " M, __FILE__, __LINE__, ##__VA_ARGS__)
#else
#define debug(M, ...)
#endif

typedef unsigned char bool;
static const bool false = 0;
static const bool true = 1;

#endif
