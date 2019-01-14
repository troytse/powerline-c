#include "git.h"

void segment_git(config_t *cfg, list_t *q)
{
    git_status_t *stat = git_status();
    if (stat != NULL)
    {
        // branch
        unsigned int branch_bg;
        unsigned int branch_fg;
        if (stat->clean)
        {
            branch_bg = config_get_int(cfg, CONF_REPO_CLEAN_BG, 2);
            branch_fg = config_get_int(cfg, CONF_REPO_CLEAN_FG, 15);
        }
        else
        {
            branch_bg = config_get_int(cfg, CONF_REPO_DIRTY_BG, 5);
            branch_fg = config_get_int(cfg, CONF_REPO_DIRTY_FG, 15);
        }
        segment_t *seg_stat = segment_create_with_default_separator(stat->branch, branch_fg, branch_bg);
        list_push(q, list_item_create(seg_stat, sizeof(segment_t)));

        // ahead
        if (stat->ahead > 0)
        {
            unsigned int bg = config_get_int(cfg, CONF_GIT_AHEAD_BG, 1);
            unsigned int fg = config_get_int(cfg, CONF_GIT_AHEAD_FG, 15);
            char *text = calloc(16, sizeof(char));
            sprintf(text, "%s %d", config_get(cfg, CONF_GIT_AHEAD, "A"), stat->ahead);
            segment_t *seg_ahead = segment_create_with_default_separator(text, fg, bg);
            list_push(q, list_item_create(seg_ahead, sizeof(segment_t)));
        }

        // behind
        if (stat->behind > 0)
        {
            unsigned int bg = config_get_int(cfg, CONF_GIT_BEHIND_BG, 1);
            unsigned int fg = config_get_int(cfg, CONF_GIT_BEHIND_FG, 15);
            char *text = calloc(16, sizeof(char));
            sprintf(text, "%s %d", config_get(cfg, CONF_GIT_BEHIND, "B"), stat->behind);
            segment_t *seg_behind = segment_create_with_default_separator(text, fg, bg);
            list_push(q, list_item_create(seg_behind, sizeof(segment_t)));
        }

        // staged
        if (stat->staged > 0)
        {
            unsigned int bg = config_get_int(cfg, CONF_GIT_STAGED_BG, 1);
            unsigned int fg = config_get_int(cfg, CONF_GIT_STAGED_FG, 15);
            char *text = calloc(16, sizeof(char));
            sprintf(text, "%s %d", config_get(cfg, CONF_GIT_STAGED, "S"), stat->staged);
            segment_t *seg_staged = segment_create_with_default_separator(text, fg, bg);
            list_push(q, list_item_create(seg_staged, sizeof(segment_t)));
        }

        // conflicted
        if (stat->conflicted > 0)
        {
            unsigned int bg = config_get_int(cfg, CONF_GIT_CONFLICTED_BG, 1);
            unsigned int fg = config_get_int(cfg, CONF_GIT_CONFLICTED_FG, 15);
            char *text = calloc(16, sizeof(char));
            sprintf(text, "%s %d", config_get(cfg, CONF_GIT_CONFLICTED, "!"), stat->conflicted);
            segment_t *seg_conflicted = segment_create_with_default_separator(text, fg, bg);
            list_push(q, list_item_create(seg_conflicted, sizeof(segment_t)));
        }

        // untracked
        if (stat->untracked > 0)
        {
            unsigned int bg = config_get_int(cfg, CONF_GIT_UNTRACKED_BG, 1);
            unsigned int fg = config_get_int(cfg, CONF_GIT_UNTRACKED_FG, 15);
            char *text = calloc(16, sizeof(char));
            sprintf(text, "%s %d", config_get(cfg, CONF_GIT_NEW, "?"), stat->untracked);
            segment_t *seg_new = segment_create_with_default_separator(text, fg, bg);
            list_push(q, list_item_create(seg_new, sizeof(segment_t)));
        }

        // changed
        if (stat->changed > 0)
        {
            unsigned int bg = config_get_int(cfg, CONF_GIT_NOTSTAGED_BG, 3);
            unsigned int fg = config_get_int(cfg, CONF_GIT_NOTSTAGED_FG, 15);
            char *text = calloc(16, sizeof(char));
            sprintf(text, "%s %d", config_get(cfg, CONF_GIT_CHANGED, "*"), stat->changed);
            segment_t *seg_mod = segment_create_with_default_separator(text, fg, bg);
            list_push(q, list_item_create(seg_mod, sizeof(segment_t)));
        }
    }
}
