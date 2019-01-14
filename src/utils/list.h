#ifndef INCLUDE_utils_list_h__
#define INCLUDE_utils_list_h__

#include "../common.h"

typedef struct list_item_s list_item_t;
struct list_item_s
{
    void *value;
    size_t value_size;
    list_item_t *prev;
    list_item_t *next;
};

typedef struct list_s list_t;
struct list_s
{
    size_t position;
    size_t length;
    list_item_t *current;
    list_item_t *front;
    list_item_t *back;
};

list_item_t *list_item_create(void *value, size_t value_size);

void list_item_destroy(list_item_t *item);

list_t *list_create();

void list_push(list_t *list, list_item_t *item);

list_item_t *list_pop(list_t *list);

void list_seek(list_t *list, size_t position);

list_item_t *list_next(list_t *list);

void list_destroy(list_t *list);

#endif