#include "git_status.h"

char *git_bin_path()
{
    static const size_t buf_size = 128;
    FILE *fp = popen("which git", "r");
    char *path = calloc(buf_size, sizeof(char));
    if (fp != NULL)
    {
        if (fgets(path, buf_size, fp) != NULL)
        {
            trim(path);
        }
        pclose(fp);
        fp = NULL;
    }
    return path;
}

void git_status_row_prase(git_status_t *stat, char *row)
{
    // X          Y     Meaning
    // -------------------------------------------------
    //   [AMD]   not updated
    // M        [ MD]   updated in index
    // A        [ MD]   added to index
    // D                deleted from index
    // R        [ MD]   renamed in index
    // C        [ MD]   copied in index
    // [MARC]           index and work tree matches
    // [ MARC]     M    work tree changed since index
    // [ MARC]     D    deleted in work tree
    // [ D]        R    renamed in work tree
    // [ D]        C    copied in work tree
    // -------------------------------------------------
    // D           D    unmerged, both deleted
    // A           U    unmerged, added by us
    // U           D    unmerged, deleted by them
    // U           A    unmerged, added by them
    // D           U    unmerged, deleted by us
    // A           A    unmerged, both added
    // U           U    unmerged, both modified
    // -------------------------------------------------
    // ?           ?    untracked
    // !           !    ignored
    // -------------------------------------------------

    size_t row_size = strlen(row);
    if (row_size < 2)
        return;

    static const char* status_conflicted = "DD,AU,UD,UA,DU,AA,UU";
    char *status = substr(row, 0, 2);
    char s = row[1] != ' ' ? row[1] : row[0];
    int ahead_pos;
    int behind_pos;

    // branch & ahead & behind
    if (row[0] == '#')
    {
        stat->branch = substr(row, 3, strpos(row, "...") - 3);
        ahead_pos = strpos(row, "[ahead ");
        behind_pos = strpos(row, "[behind");
        if (ahead_pos > 0 || behind_pos > 0)
        {
            char *tmp = substr(row, ahead_pos + behind_pos + 8, row_size - ahead_pos - behind_pos - 10);
            int value = atoi(tmp);
            if (ahead_pos > 0)
                stat->ahead = value;
            if (behind_pos > 0)
                stat->behind = value;
            free(tmp);
        }
    }
    else if (row[0] == '?')
    {
        stat->untracked++;
    }
    else if (strpos(status_conflicted, status) >= 0)
    {
        stat->conflicted++;
    }
    else
    {
        if (row[0] != ' ')
            stat->staged++;
        if (row[1] != ' ')
            stat->changed++;
        if (s == 'A')
            stat->added++;
        else if (s == 'M')
            stat->modified++;
        else if (s == 'R')
            stat->renamed++;
        else if (s == 'C')
            stat->copied++;
        else if (s == 'D')
            stat->deleted++;
        else
            fprintf(stderr, "Unknow git status: %s\n", status);
    }
}

git_status_t *git_status_create()
{
    git_status_t *stat = calloc(1, sizeof(git_status_t));
    stat->branch = NULL;
    stat->clean = true;
    stat->changed = 0;
    stat->ahead = 0;
    stat->behind = 0;
    stat->untracked = 0;
    stat->added = 0;
    stat->modified = 0;
    stat->renamed = 0;
    stat->copied = 0;
    stat->deleted = 0;
    stat->staged = 0;
    stat->conflicted = 0;
    return stat;
}

void git_status_destory(git_status_t *stat)
{
    if (stat != NULL)
    {
        if (stat->branch != NULL)
        {
            free(stat->branch);
            stat->branch = NULL;
        }
        stat->clean = true;
        stat->changed = 0;
        stat->ahead = 0;
        stat->behind = 0;
        stat->untracked = 0;
        stat->added = 0;
        stat->modified = 0;
        stat->renamed = 0;
        stat->copied = 0;
        stat->deleted = 0;
        stat->staged = 0;
        stat->conflicted = 0;
    }
}

git_status_t *git_status()
{
    static const char *args = " status -bs --porcelain 2>/dev/null";
    char *bin = git_bin_path();
    if (bin != NULL && is_executable(bin))
    {
        bin = realloc(bin, strlen(bin) + strlen(args) + 1);
        strcat(bin, args);
        git_status_t *stat = git_status_create();
        FILE *fp = popen(bin, "r");
        if (fp != NULL)
        {
            char buf[256];
            size_t count = 0;
            while (fgets(buf, sizeof(buf) - 1, fp) != NULL)
            {
                git_status_row_prase(stat, buf);
                count++;
            }
            pclose(fp);
            if (count > 0)
            {
                stat->clean = (stat->staged + stat->changed) > 0 ? false : true;
                return stat;
            }
            else
            {
                // Not a git path
                git_status_destory(stat);
            }
        }
    }
    return NULL;
}
