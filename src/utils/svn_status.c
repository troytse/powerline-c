#include "svn_status.h"

char *svn_path()
{
    static const size_t buf_size = 128;
    FILE *fp = popen("which svn", "r");
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

svn_status_t *svn_status()
{
    // TODO
    return NULL;
}
