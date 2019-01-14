#include "cwd.h"

void segment_cwd(config_t *cfg, list_t *q)
{
    static char *dir_sparator = "/";

    // get paths
    char *cwd = getenv("PWD");
    char *home = getenv("HOME");
    size_t cwd_size = strlen(cwd);
    size_t home_size = strlen(home);

    // configs
    char *mode = config_get(cfg, CONF_CWD_MODE, "default");
    int cwd_fg = config_get_int(cfg, CONF_CWD_FG, 0);
    int cwd_bg = config_get_int(cfg, CONF_CWD_BG, 15);

    char *cwd_ellipsis = config_get(cfg, CONF_CWD_ELLIPSIS, "...");
    char *cwd_separator = config_get(cfg, CONF_CWD_SEPARATOR, "/");
    int cwd_separator_fg = config_get_int(cfg, CONF_CWD_SEPARATOR_FG, 15);

    char *cwd_home_symbol = config_get(cfg, CONF_CWD_HOME_SYMBOL, "~");
    int cwd_home_fg = config_get_int(cfg, CONF_CWD_HOME_FG, 0);
    int cwd_home_bg = config_get_int(cfg, CONF_CWD_HOME_BG, 15);

    size_t max_chars = config_get_int(cfg, CONF_CWD_MAX_CHARS, 50);
    size_t max_depth = config_get_int(cfg, CONF_CWD_MAX_DEPTH, 5);

    // vars
    list_t *cwd_list;
    list_item_t *cwd_item;

    // mode
    strlwr(mode);

    // parse home path
    if (strpos(cwd, home) == 0)
    {
        if (cwd_size == home_size)
        {
            cwd = cwd_home_symbol;
            cwd_size = 1;
        }
        else
        {
            size_t new_size = strlen(cwd_home_symbol) + strlen(dir_sparator) + cwd_size - home_size - 1;
            char *tmp = calloc(new_size, sizeof(char));
            if (tmp != NULL)
            {
                sprintf(tmp, "%s%s%s",
                        cwd_home_symbol,
                        dir_sparator,
                        substr(cwd, home_size + 1, cwd_size - home_size));
                cwd = tmp;
                cwd_size = new_size;
            }
        }
    }

    // convert the cwd path to list
    cwd_list = explode(dir_sparator, cwd);

    // for plain mode
    if (strcmp(mode, "plain") == 0)
    {
        if (cwd_size > max_chars)
        {
            cwd_size = cwd_list->front->value_size +
                       cwd_list->back->value_size + (strlen(cwd_separator) * 2) + 3;
            cwd = calloc(cwd_size, sizeof(char));
            sprintf(cwd,
                    "%s%s%s%s%s",
                    (char *)cwd_list->front->value,
                    cwd_separator,
                    cwd_ellipsis,
                    cwd_separator,
                    (char *)cwd_list->back->value);
        }
        else
        {
            cwd = implode(cwd_separator, cwd_list);
            cwd_size = strlen(cwd);
        }
        segment_t *seg_cwd = segment_create_with_default_separator(cwd, cwd_fg, cwd_bg);
        list_push(q, list_item_create(seg_cwd, sizeof(segment_t)));
        return;
    }

    // for dironly
    if (strcmp(mode, "dironly") == 0)
    {
        segment_t *seg_cwd = segment_create_with_default_separator(cwd_list->back->value, cwd_fg, cwd_bg);
        list_push(q, list_item_create(seg_cwd, sizeof(segment_t)));
        return;
    }

    // home
    if (strcmp(cwd_list->front->value, cwd_home_symbol) == 0)
    {
        cwd_item = list_pop(cwd_list);
        segment_t *seg_home = segment_create_with_default_separator(cwd_item->value, cwd_home_fg, cwd_home_bg);
        list_push(q, list_item_create(seg_home, sizeof(segment_t)));
    }

    // dirs
    max_depth = 2;
    if (cwd_list->length > 0)
    {
        char *separator = calloc(20, sizeof(char));
        sprintf(separator, "\\[\e[38;5;%dm\\]%s", cwd_separator_fg, cwd_separator);

        if (cwd_list->length > max_depth)
        {
            segment_t *seg_cwd_first = segment_create(cwd_list->front->value, cwd_fg, cwd_bg, separator);
            segment_t *seg_cwd_ellipsis = segment_create(cwd_ellipsis, cwd_fg, cwd_bg, separator);
            segment_t *seg_cwd_last = segment_create_with_default_separator(cwd_list->back->value, cwd_fg, cwd_bg);
            list_push(q, list_item_create(seg_cwd_first, sizeof(segment_t)));
            list_push(q, list_item_create(seg_cwd_ellipsis, sizeof(segment_t)));
            list_push(q, list_item_create(seg_cwd_last, sizeof(segment_t)));
        }
        else
        {
            while ((cwd_item = list_next(cwd_list)) != NULL)
            {
                segment_t *seg_cwd_part;
                if (cwd_item->next != NULL)
                    seg_cwd_part = segment_create(cwd_item->value, cwd_fg, cwd_bg, separator);
                else
                    seg_cwd_part = segment_create_with_default_separator(cwd_item->value, cwd_fg, cwd_bg);
                list_push(q, list_item_create(seg_cwd_part, sizeof(segment_t)));
            }
        }
    }
}
