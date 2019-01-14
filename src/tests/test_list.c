#define DEBUG
#define TEST

#include "../common.h"
#include "../utils/list.h"
#include "test.h"

int main(int argc, char const *argv[])
{
    list_item_t *item = list_item_create("Test", 4);
    test((item != NULL &&
          item->next == NULL &&
          item->prev == NULL &&
          item->value_size == 4 &&
          strcmp(item->value, "Test") == 0),
         "Checking for list_item_create");

    if (item == NULL)
        return 1;

    list_item_destroy(item);
    test((item != NULL &&
          item->next == NULL &&
          item->prev == NULL &&
          item->value_size == 0 &&
          item->value == NULL),
         "Checking for list_item_destroy");

    list_t *list = list_create();
    test((list != NULL &&
          list->front == NULL &&
          list->back == NULL &&
          list->current == NULL &&
          list->position == 0 &&
          list->length == 0),
         "Checking for list_create");

    if (list == NULL)
        return 2;

    list_item_t *items[10];
    char buf[16];
    unsigned int i;
    for (i = 0; i < 10; i++)
    {
        sprintf(buf, "Test-%d", i);
        items[i] = list_item_create(buf, strlen(buf));
        list_push(list, items[i]);
    }
    test((list->length == 10 &&
          list->front == items[0] &&
          list->back == items[9] &&
          list->current == list->front &&
          list->position == 0),
         "Checking for list_push");

    list_seek(list, 0);
    unsigned int pass = 0;
    i = 0;
    while ((item = list_next(list)) != NULL)
    {
        if (list->position == (i + 1) &&
            strcmp(item->value, items[i]->value) == 0 &&
            item->value_size == items[i]->value_size)
            pass++;
        i++;
    }
    test(pass == 10, "Checking for list_seek & list_next");

    pass = 0;
    for (i = 0; i < 11; i++)
    {
        item = list_pop(list);
        if (item != NULL)
        {
            if (item->prev == NULL &&
                item->next == NULL &&
                strcmp(item->value, items[i]->value) == 0 &&
                item->value_size == items[i]->value_size)
                pass++;
            list_item_destroy(item);
        }
    }
    test((pass == 10 &&
          list->length == 0 &&
          list->position == 0 &&
          list->front == NULL &&
          list->back == NULL &&
          list->current == NULL),
         "Checking for list_pop");

    list_destroy(list);
    test((list != NULL &&
          list->front == NULL &&
          list->back == NULL &&
          list->current == NULL &&
          list->position == 0 &&
          list->length == 0),
         "Checking for list_destroy");

    return report_results();
}
