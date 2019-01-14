#define DEBUG
#define TEST

#include "../utils/hashtable.h"
#include "../utils/string.h"
#include "test.h"

int main(int argc, char const *argv[])
{
    hashtable_t *table = hashtable_create();
    test(table != NULL, "Checking for hashtable_create");
    if (table != NULL)
    {
        hashtable_entry_t *entry = hashtable_entry_create("FirstKey", 8, "FirstValue", 10);
        test(entry != NULL, "Checking for hashtable_entry_create");
        if (entry != NULL)
        {
            hashtable_entry_t *entry_cmp_true = hashtable_entry_create("FirstKey", 8, "FirstValue", 10);
            hashtable_entry_t *entry_cmp_false = hashtable_entry_create("SecondKey", 9, "SecondValue", 11);
            int cmp_true = hashtable_entry_key_compare(entry, entry_cmp_true);
            int cmp_false = hashtable_entry_key_compare(entry, entry_cmp_false);
            test((cmp_true && !cmp_false), "Checking for hashtable_entry_key_compare");

            hashtable_entry_set_value(entry, "SecondValue", 11);
            test((strcmp(entry->value, entry_cmp_false->value) == 0 && entry->value_size == entry_cmp_false->value_size), "Checking for hashtable_entry_set_value");

            hashtable_insert_entry(table, entry);
            test(table->key_count == 1, "Checking for hashtable_insert_entry");

            hashtable_insert(table, "SecondKey", 9, "SecondValue", 11);
            test(table->key_count == 2, "Checking for hashtable_insert");

            size_t value_size;
            char *get_value = hashtable_get(table, entry->key, entry->key_size, &value_size);
            test(strcmp(get_value, entry->value) == 0, "Checking for hashtable_get");

            hashtable_entry_destroy(entry);
            hashtable_entry_destroy(entry_cmp_true);
            hashtable_entry_destroy(entry_cmp_false);
            test((entry->key == NULL && entry->value == NULL && entry->next == NULL), "Checking for hashtable_entry_destroy");

            hashtable_erase(table);
            test((table->array != NULL && table->key_count == 0), "Checking for hashtable_erase");

            unsigned int count = 0, i = 0;

            for (i = 0; i < 64; i++)
            {
				char *key = calloc(16, sizeof(char));
				char *value = calloc(16, sizeof(char));
                sprintf(key, "Test_key_%d", i);
                sprintf(value, "Test_value_%d", i);
                hashtable_entry_t *ent = hashtable_entry_create(key, strlen(key), value, strlen(value));
                hashtable_insert_entry(table, ent);
				free(key);
				free(value);
            }
            for (i = 64; i < 128; i++)
            {
				char *key = calloc(16, sizeof(char));
				char *value = calloc(16, sizeof(char));
                sprintf(key, "Test_key_%d", i);
                sprintf(value, "Test_value_%d", i);
                hashtable_insert(table, key, strlen(key), value, strlen(value));
				free(key);
				free(value);
            }
            for (i = 0; i < 128; i++)
            {
				char *key = calloc(16, sizeof(char));
				char *value = calloc(16, sizeof(char));
                sprintf(key, "Test_key_%d", i);
                sprintf(value, "Test_value_%d", i);
                char *get_value = hashtable_get(table, key, strlen(key), &value_size);
                if (get_value != NULL && strlen(value) == value_size && strcmp(get_value, value) == 0)
                    count++;
				else
					debug("MULTI %p, %zu:%zu=%d, %s:%s=%d\n",
							get_value,
							strlen(value), value_size, strlen(value) == value_size,
							get_value, value, strcmp(get_value, value));
				free(key);
				free(value);
            }
            test((table->key_count == 128 && count == 128), "Checking for the correctness for multiple records");

            hashtable_resize(table, 128);
            count = 0;
            for (i = 0; i < 128; i++)
            {
				char *key = calloc(16, sizeof(char));
				char *value = calloc(16, sizeof(char));
                sprintf(key, "Test_key_%i", i);
                sprintf(value, "Test_value_%i", i);
                char *get_value = hashtable_get(table, key, strlen(key), &value_size);
                if (get_value != NULL && strlen(value) == value_size && strcmp(get_value, value) == 0)
                    count++;
                else
					debug("RESIZE %p, %zu:%zu=%d, %s:%s=%d\n",
							get_value,
							strlen(value), value_size, strlen(value) == value_size,
							get_value, value, strcmp(get_value, value));
				free(key);
				free(value);
            }
            test((table->array != NULL && table->array_size == 128 && table->key_count == 128 && count == 128), "Checking for hashtable_resize");

            char **keys = (char **)hashtable_keys(table);
            count = 0;
            for (i = 0; i < table->key_count; i++)
            {
                if (keys != NULL && keys[i] != NULL && strpos(keys[i], "Test_key_") == 0)
                    count++;
            }
            test(table->key_count == count, "Checking for hashtable_keys");
        } //end if

        hashtable_destroy(table);
        test((table->array_size == 0 && table->key_count == 0 && table->array == NULL), "Checking for hashtable_destroy");
    } // end if
    return report_results();
} //end main
