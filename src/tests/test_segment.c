#define DEBUG
#define TEST

#include "../common.h"
#include "../config.h"
#include "../segments/segment.h"
#include "../utils/file.h"
#include "test.h"

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Invaild arguments.\n");
        return 1;
    }
    config_t *cfg = config_load(argv[1], true);
    if (cfg == NULL)
    {
        fprintf(stderr, "Can not open the config file. (%s)\n", argv[1]);
        return 2;
    }
    unsigned int bg = config_get_int(cfg, CONF_DEFAULT_BG, 0);
    unsigned int fg = config_get_int(cfg, CONF_DEFAULT_FG, 15);
    char *separator = config_get(cfg, CONF_SEPARATOR, "/");

    segment_t *seg_n = segment_create("normal", fg, bg, separator);
    test((seg_n != NULL && strcmp(seg_n->text, "normal") == 0 && strcmp(seg_n->separator, separator) == 0 && seg_n->back_color == bg && seg_n->foeg_color == fg), "Checking for segment_create");

    segment_t *seg_s = segment_create_simple("simple");
    test((seg_s != NULL && strcmp(seg_s->text, "simple") == 0 && strcmp(seg_s->separator, separator) == 0 && seg_s->foeg_color == fg && seg_s->back_color == bg), "Checking for segment_create_simple");

    segment_t *seg_d = segment_create_with_default_separator("default_separator", fg, bg);
    test((seg_d != NULL && strcmp(seg_d->text, "default_separator") == 0 && strcmp(seg_n->separator, separator) == 0 && seg_n->foeg_color == fg && seg_n->back_color == bg), "Checking for segment_create_with_default_separator");

    return report_results();
}
