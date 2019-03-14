#ifndef HT_HASH_TABLE_H
#define HT_HASH_TABLE_H

#include <stdlib.h>

typedef struct ht_hash_table ht_hash_table;

ht_hash_table* ht_new(const size_t size);
void ht_delete(ht_hash_table* table);

void ht_insert_entry(ht_hash_table* table, const char* key, const char* value);
char* ht_search_entry(ht_hash_table* table, const char* key);
void ht_delete_entry(ht_hash_table* table, const char* key);

size_t ht_get_size(ht_hash_table* table);
size_t ht_get_count(ht_hash_table* table);

#endif
