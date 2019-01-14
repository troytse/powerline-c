#include "common.h"
#include "config.h"
#include "utils/file.h"
#include "utils/list.h"
#include "segments/segment.h"
#include "segments/username.h"
#include "segments/hostname.h"
#include "segments/cwd.h"
#include "segments/git.h"
#include "segments/cmd.h"

int main(int argc, char const *argv[])
{
    // check the arguments
    if (argc < 1 || !is_file(argv[1]))
    {
        fprintf(stderr, "Missing configuration file.\n");
        return 1;
    }
    // load configurations
    config_t *cfg = config_load(argv[1], true);
    if (cfg == NULL)
    {
        fprintf(stderr, "Can not open the config file. (%s)\n", argv[1]);
        return 2;
    }

    // get the segments
    char *segments = config_get(cfg, CONF_SEGMENTS, "username,hostname,cwd,cmd");

    // create segment List
    list_t *segment_list = list_create();

    if (strpos(segments, "username") != -1)
        segment_username(cfg, segment_list);
    if (strpos(segments, "hostname") != -1)
        segment_hostname(cfg, segment_list);
    if (strpos(segments, "cwd") != -1)
        segment_cwd(cfg, segment_list);
    if (strpos(segments, "git") != -1)
        segment_git(cfg, segment_list);
    if (strpos(segments, "cmd") != -1)
        segment_cmd(cfg, segment_list);

    // output
    segment_list_print(stdout, segment_list);
    return 0;
}
