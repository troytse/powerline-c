#ifndef INCLUDE_utils_hashtable_h__
#define INCLUDE_utils_hashtable_h__
#include "../common.h"

#define HASHTABLE_INITAL_SIZE 64

struct hashtable_entry_s
{
    void *key;
    void *value;
    size_t key_size;
    size_t value_size;
    struct hashtable_entry_s *next;
};
typedef struct hashtable_entry_s hashtable_entry_t;

struct hashtable_s
{
    size_t key_count;
    size_t array_size;
    hashtable_entry_t **array;
};
typedef struct hashtable_s hashtable_t;

// entry
hashtable_entry_t *hashtable_entry_create(void *key, size_t key_size, void *value, size_t value_size);
void hashtable_entry_destroy(hashtable_entry_t *entry);
int hashtable_entry_key_compare(hashtable_entry_t *entry1, hashtable_entry_t *entry2);
void hashtable_entry_set_value(hashtable_entry_t *entry, void *value, size_t value_size);

// table
hashtable_t *hashtable_create();
void hashtable_destroy(hashtable_t *table);
void hashtable_hash(const void *key, size_t key_size, unsigned int seed, void *out);
unsigned int hashtable_index(hashtable_t *table, void *key, size_t key_size);
void hashtable_resize(hashtable_t *table, size_t new_size);
void hashtable_insert(hashtable_t *table, void *key, size_t key_size, void *value, size_t value_size);
void hashtable_insert_entry(hashtable_t *table, hashtable_entry_t *entry);
void hashtable_erase(hashtable_t *table);
void *hashtable_get(hashtable_t *table, void *key, size_t key_size, size_t *value_size);
void **hashtable_keys(hashtable_t *table);

#endif