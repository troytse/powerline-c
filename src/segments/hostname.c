#include "hostname.h"

void segment_hostname(config_t *cfg, list_t *q)
{
    char *text = config_get(cfg, CONF_HOST_TEXT, "\\h");
    unsigned int bg = config_get_int(cfg, CONF_HOST_BG, 8);
    unsigned int fg = config_get_int(cfg, CONF_HOST_FG, 7);

    segment_t *seg = segment_create_with_default_separator(text, fg, bg);
    list_item_t *seg_item = list_item_create(seg, sizeof(segment_t));
    list_push(q, seg_item);
}
