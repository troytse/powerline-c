#include "segment.h"

segment_t *segment_create(
    char *text,
    unsigned int foeg_color,
    unsigned int back_color,
    char *separator)
{
    segment_t *seg = calloc(1, sizeof(segment_t));
    seg->foeg_color = foeg_color;
    seg->back_color = back_color;
    seg->separator = separator;
    seg->text = text;
    return seg;
}

segment_t *segment_create_with_default_separator(
    char *text,
    unsigned int foeg_color,
    unsigned int back_color)
{
    static char *default_separator = "|";
    char *separator = global_configs != NULL ? config_get(global_configs, CONF_SEPARATOR, default_separator) : default_separator;
    return segment_create(text, foeg_color, back_color, separator);
}

segment_t *segment_create_simple(char *text)
{
    unsigned int foeg_color = global_configs != NULL ? config_get_int(global_configs, CONF_DEFAULT_FG, 97) : 97;
    unsigned int back_color = global_configs != NULL ? config_get_int(global_configs, CONF_DEFAULT_BG, 41) : 41;
    return segment_create_with_default_separator(text, foeg_color, back_color);
}

void segment_list_print(FILE *fp, list_t *q)
{
    list_item_t *elem;
    list_seek(q, 0);
    while ((elem = list_next(q)) != NULL)
    {
        segment_t *seg = elem->value;
        fprintf(fp, "\\[\e[38;5;%dm\e[48;5;%dm\\] %s ", seg->foeg_color, seg->back_color, seg->text);
        if (elem->next != NULL)
        {
            segment_t *next = ((list_item_t *)elem->next)->value;
            fprintf(fp, "\\[\e[38;5;%dm\e[48;5;%dm\\]%s", seg->back_color, next->back_color, seg->separator);
        }
        else
        {
            fprintf(fp, "\\[\e[0m\e[38;5;%dm\\]%s\\[\e[0m\\]\n", seg->back_color, seg->separator);
        }
    }
}
