#include "hashtable.h"

hashtable_entry_t *hashtable_entry_create(void *key, size_t key_size, void *value, size_t value_size)
{
    // create the entry
    hashtable_entry_t *entry = calloc(1, sizeof(hashtable_entry_t));
    if (entry == NULL)
        return NULL;

    // key
    entry->key_size = key_size;
    entry->key = calloc(key_size + 1, 1);
    if (entry->key == NULL)
    {
        free(entry);
        return NULL;
    }
    memcpy(entry->key, key, key_size);

    // value
    entry->value_size = value_size;
    entry->value = calloc(value_size + 1, 1);
    if (entry->value == NULL)
    {
        free(entry->key);
        free(entry);
        return NULL;
    }
    memcpy(entry->value, value, value_size);
    entry->next = NULL;
    return entry;
}

void hashtable_entry_destroy(hashtable_entry_t *entry)
{
    if (entry == NULL)
        return;
    entry->key_size = 0;
    entry->value_size = 0;
    entry->next = NULL;
    if (entry->key != NULL)
        free(entry->key);
    entry->key = NULL;
    if (entry->value != NULL)
        free(entry->value);
    entry->value = NULL;
}

int hashtable_entry_key_compare(hashtable_entry_t *entry1, hashtable_entry_t *entry2)
{
    if (entry1->key_size != entry2->key_size)
        return 0;

    return (memcmp(entry1->key, entry2->key, entry1->key_size) == 0);
}

void hashtable_entry_set_value(hashtable_entry_t *entry, void *value, size_t value_size)
{
    if (entry->value != NULL)
        free(entry->value);
    entry->value = calloc(value_size + 1, 1);
    if (entry->value != NULL)
        memcpy(entry->value, value, value_size);
    entry->value_size = value_size;
}

hashtable_t *hashtable_create()
{
    hashtable_t *table = calloc(1, sizeof(hashtable_t));
    table->key_count = 0;
    table->array_size = HASHTABLE_INITAL_SIZE;
    table->array = calloc(table->array_size, sizeof(*(table->array)));
    if (table->array == NULL)
        return NULL;

    unsigned int i;
    for (i = 0; i < table->array_size; i++)
    {
        table->array[i] = NULL;
    }
    return table;
}

void hashtable_destroy(hashtable_t *table)
{
    unsigned int i;
    hashtable_entry_t *entry;
    hashtable_entry_t *next;

    if (table->array == NULL)
        return;
    for (i = 0; i < table->array_size; i++)
    {
        entry = table->array[i];
        while (entry != NULL)
        {
            next = entry->next;
            hashtable_entry_destroy(entry);
            entry = next;
        }
    }
    table->array_size = 0;
    table->key_count = 0;
    free(table->array);
    table->array = NULL;
}

void hashtable_hash(const void *key, size_t key_size, unsigned int seed, void *out)
{
    unsigned int hash = 0;
    unsigned int i;
    char *tmp = (char *)key;
    for (i = 0; i < key_size; i++)
    {
        hash = hash * seed + tmp[i];
    }
    hash &= 0x7FFFFFFF;
    *(unsigned int *)out = hash;
}

unsigned int hashtable_index(hashtable_t *table, void *key, size_t key_size)
{
    static const unsigned int seed = 2976579765;
    unsigned int index;
    hashtable_hash(key, key_size, seed, &index);
	index = index > 0 ? index % table->array_size : 0;
    return index;
}

void hashtable_resize(hashtable_t *table, size_t new_size)
{
    hashtable_entry_t **old_array = table->array;
    size_t old_array_size = table->array_size;

    table->array_size = new_size;
    table->array = calloc(new_size, sizeof(hashtable_entry_t *));
    table->key_count = 0;

    unsigned int i;
    for (i = 0; i < new_size; i++)
    {
        table->array[i] = NULL;
    }

    hashtable_entry_t *entry;
    hashtable_entry_t *next;
    for (i = 0; i < old_array_size; i++)
    {
        entry = old_array[i];
        while (entry != NULL)
        {
            next = entry->next;
            hashtable_insert_entry(table, entry);
            entry = next;
        }
        old_array[i] = NULL;
    }
    free(old_array);
}

void hashtable_insert(hashtable_t *table, void *key, size_t key_size, void *value, size_t value_size)
{
    hashtable_entry_t *entry = hashtable_entry_create(key, key_size, value, value_size);
    hashtable_insert_entry(table, entry);
}

void hashtable_insert_entry(hashtable_t *table, hashtable_entry_t *entry)
{
    hashtable_entry_t *tmp;
    unsigned int index;
    entry->next = NULL;
    index = hashtable_index(table, entry->key, entry->key_size);
    tmp = table->array[index];

    if (tmp == NULL)
    {
        table->array[index] = entry;
        table->key_count++;
        return;
    }
    while (tmp->next != NULL)
    {
        if (hashtable_entry_key_compare(tmp, entry))
            break;
        else
            tmp = tmp->next;
    }
    if (hashtable_entry_key_compare(tmp, entry))
    {
        hashtable_entry_set_value(tmp, entry->value, entry->value_size);
        entry = tmp;
    }
    else
    {
        tmp->next = entry;
        table->key_count++;
    }
}

void hashtable_erase(hashtable_t *table)
{
    unsigned int i;
    hashtable_entry_t *entry;
    hashtable_entry_t *next;

    if (table->array == NULL)
        return;
    for (i = 0; i < table->array_size; i++)
    {
        entry = table->array[i];
        while (entry != NULL)
        {
            next = entry->next;
            hashtable_entry_destroy(entry);
            entry = next;
        }
        table->array[i] = NULL;
    }
    table->key_count = 0;
}

void *hashtable_get(hashtable_t *table, void *key, size_t key_size, size_t *value_size)
{
    unsigned int index = hashtable_index(table, key, key_size);
    hashtable_entry_t *entry = table->array[index];
    hashtable_entry_t *compare = hashtable_entry_create(key, key_size, NULL, 0);
    void *value = NULL;
    while (entry != NULL)
    {
        if (hashtable_entry_key_compare(entry, compare))
        {
            if (value_size != NULL)
                *value_size = entry->value_size;
            value = entry->value;
            break;
        }
        else
        {
            entry = entry->next;
        }
    }
    hashtable_entry_destroy(compare);
    return value;
}

void **hashtable_keys(hashtable_t *table)
{
    unsigned int i, j = 0;
    hashtable_entry_t *entry;
    void **keys = calloc(table->key_count, sizeof(void *));
    for (i = 0; i < table->array_size; i++)
    {
        entry = table->array[i];
        while (entry != NULL)
        {
            keys[j++] = entry->key;
            entry = entry->next;
        }
    }
    return keys;
}
