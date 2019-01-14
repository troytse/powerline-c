#define DEBUG
#define TEST

#include "../common.h"
#include "../config.h"
#include "../utils/file.h"
#include "test.h"

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Invaild arguments.\n");
        return 1;
    }

    FILE *fp = fopen(argv[1], "w+t");
    if (fp == NULL)
    {
        fprintf(stderr, "Can not open the config file. (%s)\n", argv[1]);
        return 2;
    }
    char *tmp = "test_config=sample\ntest_unescape=\\xee\\x82\\xb0\ntest_int=123\ntest_float=1.23\ntest_bool=true\ntest_bool2=On\ntest_bool3=yes\ntest_bool4=123\ntest_bool5=1.23";
    size_t length = strlen(tmp);
    size_t writeten = fwrite(tmp, sizeof(char), length, fp);
    debug("\nTest data (Length: %zu):\n---------------------------------\n%s\n---------------------------------\n", length, tmp);
    test(writeten == length, "Checking for create test data (%s).", argv[1]);
    fflush(fp);
    fclose(fp);
    if (writeten == length)
    {
        config_load(argv[1], true);
        test(global_configs != NULL, "Checking for config_load(global).");
        config_t *cfg = config_load(argv[1], false);
        test(cfg != NULL, "Checking for config_load.");

        if (cfg != NULL)
        {
            test(cfg->data != NULL, "Checking for internal hashtable init.");
            if (cfg->data != NULL)
            {
                test(cfg->count == cfg->data->key_count, "Checking for internal counter.");

                char *k1 = "test_config";
                char *v1 = "sample";
                char *k2 = "test_unescape";
                char *v2 = "\xee\x82\xb0";

                char *r1 = config_get(cfg, k1, NULL);
                char *r2 = config_get(cfg, k2, NULL);

                test((r1 != NULL && strcmp(v1, r1) == 0), "Checking for config_get: %s", r1);
                test((r2 != NULL && strcmp(v2, r2) == 0), "Checking for config_get with unescape value: %s", r2);

                char *k3 = "test_save";
                char *v3 = "123456";

                config_set(cfg, k3, v3);
                config_save(cfg, argv[1]);
                cfg = config_load(argv[1], false);
                char *r3 = cfg != NULL ? config_get(cfg, k3, NULL) : NULL;

                test((r3 != NULL && strcmp(v3, r3) == 0), "Checking for config_save");

                char *ki = "test_int";
                double vi = 123;
                char *kd = "test_float";
                double vd = 1.23;
                char *kb = "test_bool";
                double vb = true;

                config_set_int(cfg, ki, vi);
                config_set_float(cfg, kd, vd);
                config_set_bool(cfg, kb, vb);
                config_save(cfg, argv[1]);
                cfg = config_load(argv[1], false);

                int ri = config_get_int(cfg, "test_int", 0);
                double rd = config_get_float(cfg, "test_float", 0);
                bool rb = config_get_bool(cfg, "test_bool", false);

                test(ri == vi, "Checking for config_set_int & config_save & config_get_int: %d", ri);
                test(rd == vd, "Checking for config_set_float & config_save & config_get_float: %f", rd);
                test(rb == vb, "Checking for config_set_bool & config_save & config_get_bool: %s", rb ? "True" : "False");

                bool rb2 = config_get_bool(cfg, "test_bool2", false);
                test(rb2 == true, "Checking for config_get_bool using 'On/Off': %s", rb2 ? "True" : "False");
                bool rb3 = config_get_bool(cfg, "test_bool3", false);
                test(rb3 == true, "Checking for config_get_bool using 'yes/no': %s", rb3 ? "True" : "False");
                bool rb4 = config_get_bool(cfg, "test_bool4", false);
                test(rb4 == true, "Checking for config_get_bool using int: %s", rb4 ? "True" : "False");
                bool rb5 = config_get_bool(cfg, "test_bool5", false);
                test(rb5 == true, "Checking for config_get_bool using float: %s", rb5 ? "True" : "False");

                char* rnc = config_get(cfg, "test_not_exist", NULL);
                test(rnc == NULL, "Checking for config_get(not exist): %s", rnc);

                int rni = config_get_int(cfg, "test_not_exist", 2);
                test(rni == 2, "Checking for config_get_int(not exist): %d", rni);

                double rnd = config_get_float(cfg, "test_not_exist", 0.1);
                test(rnd == 0.1, "Checking for config_get_float(not exist): %f", rnd);

                bool rnb = config_get_bool(cfg, "test_not_exist", true);
                test(rnb == true, "Checking for config_get_bool(not exist): %s", rnb ? "True" : "False");
            }
        }
    }
    return report_results();
}
