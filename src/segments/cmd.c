#include "cmd.h"

void segment_cmd(config_t *cfg, list_t *q)
{
    unsigned int uid = getuid();
    unsigned int cmd_fg;
    unsigned int cmd_bg;
    char *cmd_symbol;

    if (uid == 0)
    {
        cmd_symbol = config_get(cfg, CONF_CMD_ROOT_SYMBOL, "#");
        cmd_bg = config_get_int(cfg, CONF_CMD_ROOT_BG, 9);
        cmd_fg = config_get_int(cfg, CONF_CMD_ROOT_FG, 15);
    }
    else
    {
        cmd_symbol = config_get(cfg, CONF_CMD_USER_SYMBOL, "$");
        cmd_bg = config_get_int(cfg, CONF_CMD_USER_BG, 8);
        cmd_fg = config_get_int(cfg, CONF_CMD_USER_FG, 15);
    }

    segment_t *seg = segment_create_with_default_separator(cmd_symbol, cmd_fg, cmd_bg);
    list_item_t *seg_item = list_item_create(seg, sizeof(segment_t));
    list_push(q, seg_item);
}
