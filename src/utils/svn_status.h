#ifndef INCLUDE_utils_svn_status_h
#define INCLUDE_utils_svn_status_h

#include "../common.h"
#include "file.h"
#include "string.h"

struct svn_status_s
{
    bool clean;
};
typedef struct svn_status_s svn_status_t;

char* svn_path();

svn_status_t *svn_status();

#endif