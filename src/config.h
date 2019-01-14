#ifndef INCLUDE_config_h__
#define INCLUDE_config_h__
#include "common.h"
#include "utils/hashtable.h"

#define CONF_DEFAULT_BG             "general.default_bg"
#define CONF_DEFAULT_FG             "general.default_fg"

#define CONF_SEGMENTS               "general.segments"
#define CONF_SEPARATOR              "general.separator"

#define CONF_USER_TEXT              "username.text"
#define CONF_USER_FG                "username.fg"
#define CONF_USER_BG                "username.bg"
#define CONF_ROOT_TEXT              "username.root_text"
#define CONF_ROOT_BG                "username.root_bg"
#define CONF_ROOT_FG                "username.root_fg"

#define CONF_HOST_TEXT              "hostname.text"
#define CONF_HOST_FG                "hostname.fg"
#define CONF_HOST_BG                "hostname.bg"

#define CONF_CWD_MODE               "cwd.mode"
#define CONF_CWD_MAX_DEPTH          "cwd.max_depth"
#define CONF_CWD_MAX_CHARS          "cwd.max_chars"
#define CONF_CWD_SEPARATOR          "cwd.separator"
#define CONF_CWD_ELLIPSIS           "cwd.ellipsis"
#define CONF_CWD_SEPARATOR_FG       "cwd.separator_fg"
#define CONF_CWD_HOME_SYMBOL        "cwd.home_symbol"
#define CONF_CWD_HOME_BG            "cwd.home_bg"
#define CONF_CWD_HOME_FG            "cwd.home_fg"
#define CONF_CWD_BG                 "cwd.bg"
#define CONF_CWD_FG                 "cwd.fg"

// for cmd
#define CONF_CMD_USER_SYMBOL        "cmd.user_symbol"
#define CONF_CMD_USER_BG            "cmd.user_bg"
#define CONF_CMD_USER_FG            "cmd.user_fg"
#define CONF_CMD_ROOT_SYMBOL        "cmd.root_symbol"
#define CONF_CMD_ROOT_BG            "cmd.root_bg"
#define CONF_CMD_ROOT_FG            "cmd.root_fg"

// repos commons
#define CONF_REPO_CLEAN_BG          "repo.clean_bg"
#define CONF_REPO_CLEAN_FG          "repo.clean_fg"
#define CONF_REPO_DIRTY_BG          "repo.dirty_bg"
#define CONF_REPO_DIRTY_FG          "repo.dirty_fg"

#define CONF_SVN_BG                 "svn.changes_bg"
#define CONF_SVN_FG                 "svn.changes_fg"

#define CONF_GIT_DETACHED           "git.detached"
#define CONF_GIT_AHEAD              "git.ahead"
#define CONF_GIT_BEHIND             "git.behind"
#define CONF_GIT_STAGED             "git.staged"
#define CONF_GIT_CHANGED            "git.changed"
#define CONF_GIT_NEW                "git.new"
#define CONF_GIT_CONFLICTED         "git.conflicted"
#define CONF_GIT_AHEAD_BG           "git.ahead_bg"
#define CONF_GIT_AHEAD_FG           "git.ahead_fg"
#define CONF_GIT_BEHIND_BG          "git.behind_bg"
#define CONF_GIT_BEHIND_FG          "git.behind_fg"
#define CONF_GIT_STAGED_BG          "git.staged_bg"
#define CONF_GIT_STAGED_FG          "git.staged_fg"
#define CONF_GIT_NOTSTAGED_BG       "git.notstaged_bg"
#define CONF_GIT_NOTSTAGED_FG       "git.notstaged_fg"
#define CONF_GIT_UNTRACKED_BG       "git.untracked_bg"
#define CONF_GIT_UNTRACKED_FG       "git.untracked_fg"
#define CONF_GIT_CONFLICTED_BG      "git.conflicted_bg"
#define CONF_GIT_CONFLICTED_FG      "git.conflicted_fg"

struct config_s
{
    size_t count;
    hashtable_t *data;
};
typedef struct config_s config_t;

extern config_t *global_configs;

config_t *config_load(const char *path, bool set_global);

void config_set(config_t *cfg, const char *key, char *value);

void config_set_int(config_t *cfg, const char *key, int value);

void config_set_float(config_t *cfg, const char *key, double value);

void config_set_bool(config_t *cfg, const char *key, bool value);

char *config_get(config_t *cfg, const char *key, char *default_value);

int config_get_int(config_t *cfg, const char *key, int default_value);

double config_get_float(config_t *cfg, const char *key, double default_value);

bool config_get_bool(config_t *cfg, const char *key, bool default_value);

bool config_save(config_t *cfg, const char *path);

void config_destroy(config_t *cfg);

void config_print(FILE *fp, config_t *cfg);

#endif
