#include "config.h"
#include "utils/hashtable.h"
#include "utils/file.h"
#include "utils/string.h"

config_t *global_configs;

config_t *config_load(const char *path, bool set_global)
{
    config_t *cfg;
    if (set_global == true)
    {
        if (global_configs != NULL)
        {
            config_destroy(global_configs);
        }
        cfg = global_configs = calloc(1, sizeof(config_t));
    }
    else
    {
        cfg = calloc(1, sizeof(config_t));
    }
    cfg->data = hashtable_create();
    FILE* fp = fopen(path, "r");
    if (fp != NULL && cfg->data != NULL)
    {
        const char *config_delim = "=";
        char buf[256];
        char *config_key;
        char *config_value;
        char *p;
        bool in_single_quotes = false;
        bool in_double_quotes = false;
        list_t *items;

        while (fgets(buf, 255, fp) != NULL)
        {
            items = explode(config_delim, buf);
            if (items->length != 2)
            {
                list_destroy(items);
                continue;
            }
            config_key = items->front->value;
            config_value = items->back->value;
            p = config_value;
            while (*p)
            {
                if (*p == '"')
                {
                    in_double_quotes = !in_double_quotes;
                    if (!in_double_quotes)
                    {
                        *p = '\0';
                        break;
                    }
                }
                else if (*p == '\'')
                {
                    in_single_quotes = !in_single_quotes;
                    if (!in_single_quotes)
                    {
                        *p = '\0';
                        break;
                    }
                }
                if (*p == '#' && !in_double_quotes && !in_single_quotes)
                {
                    *p = '\0';
                    break;
                }
                p++;
            }
            trim_multi(config_value, "\"'", 2);
            trim(config_value);
            trim(config_key);
            strlwr(config_key);

            if (strpos(config_value, "\\") >= 0)
                unescape(config_value);

            config_set(cfg, config_key, config_value);
            list_destroy(items);
        }
        fclose(fp);
        return cfg;
    }
    return NULL;
}

void config_set(config_t *cfg, const char *key, char *value)
{
	if (cfg != NULL && cfg->data != NULL)
	{
		hashtable_t *table = cfg->data;
		hashtable_insert(cfg->data, (void *)key, strlen(key), value, strlen(value));
		cfg->count = table->key_count;
	}
}

void config_set_int(config_t *cfg, const char *key, int value)
{
	if (cfg != NULL && cfg->data != NULL)
	{
		hashtable_t *table = cfg->data;
		char *value_str = calloc(32, sizeof(char));
		sprintf(value_str, "%d", value);
		hashtable_insert(cfg->data, (void *)key, strlen(key), value_str, strlen(value_str));
		cfg->count = table->key_count;
		free(value_str);
	}
}

void config_set_float(config_t *cfg, const char *key, double value)
{
	if (cfg != NULL && cfg->data != NULL)
	{
		hashtable_t *table = cfg->data;
		char *value_str = calloc(32, sizeof(char));
		sprintf(value_str, "%f", value);
		hashtable_insert(cfg->data, (void *)key, strlen(key), value_str, strlen(value_str));
		cfg->count = table->key_count;
		free(value_str);
	}
}

void config_set_bool(config_t *cfg, const char *key, bool value)
{
    static const char bool_str[2][5] = {"false", "true"};
	if (cfg != NULL && cfg->data != NULL)
	{
		hashtable_t *table = cfg->data;
		size_t value_size = strlen(bool_str[value]);
		char *value_str = calloc(value_size, sizeof(char));
		memcpy(value_str, bool_str[value], value_size);
		hashtable_insert(cfg->data, (void *)key, strlen(key), value_str, value_size);
		cfg->count = table->key_count;
		free(value_str);
	}
}

char *config_get(config_t *cfg, const char *key, char *default_value)
{
    char *value = hashtable_get(cfg->data, (void *)key, strlen(key), NULL);
    return value == NULL ? default_value : value;
}

int config_get_int(config_t *cfg, const char *key, int default_value)
{
    static const char *pattern = "^[0-9]+$";
    static const size_t nmatch = 1;
    static regmatch_t pmatch[1];
    static regex_t regex;
    static int status;

    char *value = config_get(cfg, key, NULL);
    int result = default_value;
    if (value != NULL)
    {
        status = regcomp(&regex, pattern, REG_EXTENDED | REG_NEWLINE);
        status = regexec(&regex, value, nmatch, pmatch, 0);
        if (status == 0)
            result = atoi(value);
        regfree(&regex);
    }
    else
    {
        return default_value;
    }
    return result;
}

double config_get_float(config_t *cfg, const char *key, double default_value)
{
    static const char *pattern = "^[0-9]+|[0-9]+\\.[0-9]+$";
    static const size_t nmatch = 1;
    static regmatch_t pmatch[1];
    static regex_t regex;
    static int status;

    char *value = config_get(cfg, key, 0);
    double result = default_value;
    if (value != NULL)
    {
        status = regcomp(&regex, pattern, REG_EXTENDED | REG_NEWLINE);
        status = regexec(&regex, value, nmatch, pmatch, 0);
        if (status == 0)
            result = atof(value);
        regfree(&regex);
    }
    else
    {
        return default_value;
    }
    return result;
}

bool config_get_bool(config_t *cfg, const char *key, bool default_value)
{
    static const char *pattern = "^[1-9]+|[1-9][0-9]+|true|on|yes$";
    static const size_t nmatch = 1;
    static regmatch_t pmatch[1];
    static regex_t regex;
    static int status;

    char *value = config_get(cfg, key, 0);
    bool result = default_value;
    if (value != NULL)
    {
        status = regcomp(&regex, pattern, REG_EXTENDED | REG_ICASE | REG_NEWLINE);
        status = regexec(&regex, value, nmatch, pmatch, 0);
		result = status == 0;
        regfree(&regex);
    }
    else
    {
        return default_value;
    }
    return result;
}

bool config_save(config_t *cfg, const char *path)
{
    FILE *fp = fopen(path, "w+t");
    if (fp != NULL)
    {
        config_print(fp, cfg);
        fflush(fp);
        fclose(fp);
        return true;
    }
    return false;
}

void config_destroy(config_t *cfg)
{
    hashtable_destroy(cfg->data);
    cfg->data = NULL;
    cfg->count = 0;
    free(cfg);
}

void config_print(FILE *fp, config_t *cfg)
{
    if (fp != NULL && cfg != NULL)
    {
        unsigned int i;
        hashtable_entry_t *entry;
        for (i = 0; i < cfg->data->array_size; i++)
        {
            entry = cfg->data->array[i];
            while (entry != NULL)
            {
                fprintf(fp, "%s=%s\n", (char *)entry->key, (char *)entry->value);
                entry = entry->next;
            }
        }
    }
}
