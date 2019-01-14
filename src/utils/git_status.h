#ifndef INCLUDE_utils_git_status_h
#define INCLUDE_utils_git_status_h

#include "../common.h"
#include "file.h"
#include "string.h"

struct git_status_s
{
    char* branch;
    bool clean;
    unsigned int changed;
    unsigned int ahead;
    unsigned int behind;
    unsigned int untracked;
    unsigned int added;
    unsigned int modified;
    unsigned int renamed;
    unsigned int copied;
    unsigned int deleted;
    unsigned int staged;
    unsigned int conflicted;
};
typedef struct git_status_s git_status_t;

void git_status_row_prase(git_status_t *stat, char *row);

git_status_t *git_status_create();

void git_status_destory(git_status_t *stat);

char* git_bin_path();

git_status_t *git_status();

#endif