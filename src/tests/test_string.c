#define DEBUG
#define TEST

#include "../common.h"
#include "../utils/list.h"
#include "../utils/string.h"
#include "test.h"

int main(int argc, char const *argv[])
{
    char *s1 = "strpos..test";
    test(strpos(s1, "..") == 6, "Checking for strpos");
    test(strrpos(s1, "pos") == 3, "Checking for strrpos");

    char *s2 = substr(s1, 1, 3);
    test(strcmp(s2, "trp") == 0, "Checking for substr");

    char s3[] = " trim test\t ";
    trim(s3);
    test(strcmp(s3, "trim test") == 0, "Checking for trim");

    char s4[] = "\ttrim test\t";
    trim_chr(s4, '\t');
    test(strcmp(s4, "trim test") == 0, "Checking for trim_chr");

    char s5[] = "\ttrim test\t";
    trim_multi(s5, "\tst", 3);
    test(strcmp(s5, "rim te") == 0, "Checking for trim_multi");

    char *s6 = "aaa,bbb,ccc";
    char *s6_cmp[3] = {"aaa", "bbb", "ccc"};
    list_t *s6_exploded = explode(",", s6);
    list_item_t *s6_item;
    list_seek(s6_exploded, 0);
    size_t pass = 0;
    unsigned int i = 0;
    while((s6_item = list_next(s6_exploded)) != NULL) {
        if (s6_item->value != NULL && strcmp(s6_item->value, s6_cmp[i++]) == 0 && s6_item->value_size == 3)
            pass++;
    }
    test(pass == 3, "Checking for explode");

    char *s7 = implode("|", s6_exploded);
    char *s7_cmp = "aaa|bbb|ccc";
    test(strcmp(s7, s7_cmp) == 0, "Checking for implode");

    return report_results();
}