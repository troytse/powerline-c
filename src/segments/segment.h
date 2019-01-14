#ifndef INCLUDE_segments_segment_h__
#define INCLUDE_segments_segment_h__
#include "../common.h"
#include "../config.h"
#include "../utils/list.h"
#include "../utils/string.h"

struct segment_s
{
    unsigned int foeg_color;
    unsigned int back_color;
    char *separator;
    char *text;
};
typedef struct segment_s segment_t;

segment_t *segment_create(
    char *text,
    unsigned int foeg_color,
    unsigned int back_color,
    char *separator);

segment_t *segment_create_with_default_separator(
    char *text,
    unsigned int foeg_color,
    unsigned int back_color);

segment_t *segment_create_simple(char *text);

void segment_destroy(segment_t *seg);

void segment_list_print(FILE *fp, list_t *q);

#endif