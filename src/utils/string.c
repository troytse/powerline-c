#include "string.h"

int strpos(const char *haystack, char *needle)
{
    unsigned int i, j;
    for (i = 0; haystack[i] != '\0'; i++)
    {
        if (haystack[i] != needle[0])
            continue;
        j = 0;
        while (needle[j] != '\0' && haystack[i + j] != '\0')
        {
            j++;
            if (needle[j] != haystack[i + j])
                break;
        }
        if (needle[j] == '\0')
            return i;
    }
    return -1;
}

int strrpos(const char *haystack, char *needle)
{
    size_t i = strlen(haystack), j = 0;
    while (i-- > 0)
    {
        if (haystack[i] != needle[0])
            continue;
        j = 0;
        while (needle[j] != '\0' && haystack[i + j] != '\0')
        {
            j++;
            if (needle[j] != haystack[i + j])
                break;
        }
        if (needle[j] == '\0')
            return i;
    }
    return -1;
}

char *substr(const char *str, int start, int length)
{
    int end = start + length;
    size_t max = strlen(str);
    char *buf = (char *)calloc(length + 1, sizeof(char));

    if (start >= max)
    {
        return (char *)NULL;
    }

    for (int i = 0, j = start; i < max && j < end; i++, j++)
    {
        if (str[j] == '\0')
            break;
        buf[i] = str[j];
    }
    return buf;
}

void trim(char *str)
{
    static const char charlist[] = {'\0', '\t', '\n', '\x0B', '\r', ' '};
    trim_multi(str, charlist, 6);
}

void trim_chr(char *str, const char chr)
{
    const char *charlist = {&chr};
    trim_multi(str, charlist, 1);
}

void trim_multi(char *str, const char *charlist, size_t charlist_size)
{
    char *lft = str;
    char *rgt = str;
    char *o_lft = NULL;
    char *o_rgt = NULL;
    unsigned int i;

    while (*rgt)
        rgt++;
    rgt--;

    while (o_lft != lft || o_rgt != rgt)
    {
        o_lft = lft;
        o_rgt = rgt;
        for (i = 0; i < charlist_size; i++)
        {
            if (*lft == charlist[i])
                lft++;
            if (*rgt == charlist[i])
                rgt--;
        }
    }

    while (lft <= rgt)
        *(str++) = *lft++;
    while (*str)
        *(str++) = 0;
}

void strlwr(char *str)
{
    while (*str != '\0')
    {
        if (*str >= 'A' && *str <= 'Z')
            *str += 32;
        str++;
    }
}

void strupr(char *str)
{
    while (*str != '\0')
    {
        if (*str >= 'a' && *str <= 'z')
            *str -= 32;
        str++;
    }
}

int hex2dec(char chr)
{
    if (chr >= '0' && chr <= '9')
        return chr - 48;
    else if (chr >= 'a' && chr <= 'f')
        return chr - 97 + 10;
    else if (chr >= 'A' && chr <= 'F')
        return chr - 65 + 10;
    return 0;
}

int unescape(char *str)
{
    static char escape[256] = {
        ['a'] = '\a',
        ['b'] = '\b',
        ['f'] = '\f',
        ['n'] = '\n',
        ['r'] = '\r',
        ['t'] = '\t',
        ['v'] = '\v',
        ['\\'] = '\\',
        ['\''] = '\'',
        ['"'] = '\"',
        ['?'] = '\?',
    };

    char *p = str; /* Pointer to original string */
    char *q = str; /* Pointer to new string; q <= p */

    while (*p)
    {
        int c = *(unsigned char *)p++;

        if (c == '\\')
        {
            c = *(unsigned char *)p++;
            if (c == '\0')
                break;
            if (escape[c])
                c = escape[c];
            if (c == 'x')
            {
                c = hex2dec(*(unsigned char *)p++) * 16;
                c = c + hex2dec(*(unsigned char *)p++);
            }
        }

        *q++ = c;
    }
    *q = '\0';

    return q - str;
}

list_t *explode(const char *delim, char *str)
{
    list_t *list = calloc(1, sizeof(list_t));
    char *buf, *token;
    if (list != NULL)
    {
        buf = (char *)strdup(str);
        token = strtok(buf, delim);
        while (token != NULL)
        {
            list_item_t *item = list_item_create(token, strlen(token));
            if (item != NULL)
                list_push(list, item);
            token = strtok(NULL, delim);
        }
        free(buf);
    }
    return list;
}

char *implode(const char *delim, list_t *list)
{
    char *result = NULL;
    char *prev_result = NULL;
    list_item_t *item;
    size_t delim_size = strlen(delim);
    size_t current_size = 0;

    if (list != NULL)
    {
        list_seek(list, 0);
        while ((item = list_next(list)) != NULL)
        {
            if (result != NULL)
                prev_result = result;
            result = calloc(item->value_size + current_size + delim_size + 1, sizeof(char));
            if (result != NULL)
            {
                if (prev_result != NULL)
                    strcat(result, prev_result);
                strcat(result, item->value);
                if (item->next != NULL)
                {
                    strcat(result, delim);
                    current_size += delim_size;
                }
                current_size += item->value_size;
            }
            if (prev_result != NULL)
            {
                free(prev_result);
                prev_result = NULL;
            }
        }
    }
    return result;
}
