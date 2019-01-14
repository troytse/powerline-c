#include "list.h"

list_item_t *list_item_create(void *value, size_t value_size)
{
    list_item_t *item = calloc(1, sizeof(list_item_t));
    if (item != NULL)
    {
        item->value = calloc(value_size + 1, sizeof(void));
        if (item->value != NULL)
        {
            item->value_size = value_size;
            memcpy(item->value, value, value_size);
        }
    }
    return item;
}

void list_item_destroy(list_item_t *item)
{
    if (item != NULL)
    {
        if (item->value != NULL)
            free(item->value);
        item->value = NULL;
        item->value_size = 0;
    }
}

list_t *list_create()
{
    list_t *list = calloc(1, sizeof(list_t));
    if (list != NULL)
    {
        list->position = 0;
        list->length = 0;
        list->current = list->front = list->back = NULL;
    }
    return list;
}

void list_push(list_t *list, list_item_t *item)
{
    if (list != NULL)
    {
        if (list->back != NULL)
        {
            // append to the end of list
            list->back->next = item;
            item->prev = list->back;
            item->next = NULL;
            list->back = item;
        }
        else
        {
            // first itement
            list->position = 0;
            list->current = item;
            list->front = item;
            list->back = item;
            item->prev = NULL;
            item->next = NULL;
        }
        list->length++;
    }
}

list_item_t *list_pop(list_t *list)
{
    list_item_t *item = NULL;
    if (list != NULL)
    {
        if (list->front != NULL)
        {
            item = list->front;
            if (item->next != NULL) {
                list->front = item->next;
                list->front->prev = NULL;
            } else {
                list->front = list->back = NULL;
            }
            list->current = list->front;
            list->position = 0;
            list->length--;
            item->prev = NULL;
            item->next = NULL;
        }
    }
    return item;
}

list_item_t *list_next(list_t *list)
{
    list_item_t *item = NULL;
    if (list != NULL)
    {
        if (list->current != NULL)
        {
            item = list->current;
            if (list->current != NULL)
                list->position++;
            list->current = list->current->next;
        }
    }
    return item;
}

void list_seek(list_t *list, size_t position)
{
    if (list != NULL)
    {
        if (list->length == 0)
        {
            list->position = 0;
            list->current = NULL;
        }
        else if (position >= list->length)
        {
            list->position = list->length - 1;
            list->current = list->back;
        }
        else
        {
            list->position = position;
            unsigned int i;
            list->current = list->front;
            for (i = 0; i < position; i++)
                list->current = list->current->next;
        }
    }
}

void list_destroy(list_t *list)
{
    if (list != NULL)
    {
        list_item_t *item = list->front;
        while (item != NULL)
        {
            list_item_destroy(item);
            item = item->next;
        }
        list->current = list->front = list->back = NULL;
        list->length = 0;
        list->position = 0;
    }
}
