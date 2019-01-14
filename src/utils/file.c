#include "file.h"

bool is_file(const char* path)
{
    struct stat s;
    return (stat(path, &s) == 0 && S_ISREG(s.st_mode)) ? true : false;
}

bool is_dir(const char* path)
{
    struct stat s;
    return (stat(path, &s) == 0 && S_ISDIR(s.st_mode)) ? true : false;
}

bool is_link(const char* path)
{
    struct stat s;
    return (stat(path, &s) == 0 && S_ISLNK(s.st_mode)) ? true : false;
}

bool is_executable(const char* path)
{
    struct stat s;
    return (stat(path, &s) == 0 && (s.st_mode & S_IXUSR) == S_IXUSR) ? true : false;
}

bool file_exists(const char* path)
{
    FILE* fp = fopen(path, "r");
    if (fp != NULL)
    {
        fclose(fp);
        fp = NULL;
        return true;
    }
    return false;
}

char *get_program_path()
{
    static const char* cmd = "/proc/self/exe";
    char buf[1024];
    if (readlink(cmd, buf, sizeof(buf)))
    {
        size_t result_size = strlen(buf);
        char *result = calloc(result_size, sizeof(char));
        memcpy(result, buf, result_size);
        return result;
    }
    return NULL;
}
