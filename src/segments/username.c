#include "username.h"

void segment_username(config_t *cfg, list_t *q)
{
    static char *default_text = "\\u";
    unsigned int uid = getuid();
    unsigned int bg;
    unsigned int fg;
    char *text;

    if (uid == 0)
    {
        text = config_get(cfg, CONF_ROOT_TEXT, default_text);
        bg = config_get_int(cfg, CONF_ROOT_BG, 9);
        fg = config_get_int(cfg, CONF_ROOT_FG, 15);
    }
    else
    {
        text = config_get(cfg, CONF_USER_TEXT, default_text);
        bg = config_get_int(cfg, CONF_USER_BG, 8);
        fg = config_get_int(cfg, CONF_USER_FG, 7);
    }

    segment_t *seg = segment_create_with_default_separator(text, fg, bg);
    list_item_t *seg_item = list_item_create(seg, sizeof(segment_t));
    list_push(q, seg_item);
}
