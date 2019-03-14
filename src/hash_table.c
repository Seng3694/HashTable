#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "hash_table.h"
#include "prime.h"

typedef struct
{
	char* key;
	char* value;
} ht_key_value_pair;

struct ht_hash_table
{
	size_t size;
	size_t count;
	ht_key_value_pair** items;
};

static const size_t HT_MIN_SIZE = 31;
static ht_key_value_pair HT_DELETED_ITEM = { NULL, NULL };
static const int HT_PRIME_1 = 49943;
static const int HT_PRIME_2 = 1327;

static ht_key_value_pair* ht_new_item(const char* key, const char* value);
static void ht_delete_item(ht_key_value_pair* item);
static int ht_hash(const char* str, const int prime, const size_t buckets);
static int ht_get_hash(const char* str, const size_t buckets, const int attempt);
static void ht_resize(ht_hash_table* table, const size_t size);
static void ht_resize_up(ht_hash_table* table);
static void ht_resize_down(ht_hash_table* table);

static ht_key_value_pair* ht_new_item(const char* key, const char* value)
{
	ht_key_value_pair* item = malloc(sizeof(ht_key_value_pair));
	item->key = _strdup(key);
	item->value = _strdup(value);
	return item;
}

static void ht_delete_item(ht_key_value_pair* item)
{
	if (item == &HT_DELETED_ITEM) return;

	free(item->key);
	free(item->value);
	free(item);
}

static int ht_hash(const char* str, const int prime, const size_t buckets)
{
	long hash = 0;
	const size_t length = strlen(str);
	size_t i = 0;

	for (i = 0; i < length; ++i)
	{
		hash += (long)pow((double)prime, (double)(length - (i + 1))) * str[i];
		hash %= (long)buckets;
	}

	return (int)hash;
}

static int ht_get_hash(const char* str, const size_t buckets, const int attempt)
{
	const int hash_a = ht_hash(str, HT_PRIME_1, buckets);
	const int hash_b = ht_hash(str, HT_PRIME_2, buckets);
	return (hash_a + (attempt * (hash_b + 1))) % buckets;
}

ht_hash_table* ht_new(const size_t size)
{
	ht_hash_table* table = malloc(sizeof(ht_hash_table));
	table->size = size < HT_MIN_SIZE ? HT_MIN_SIZE : (size_t)ht_next_prime((int)size);
	table->count = 0;
	table->items = calloc(table->size, sizeof(ht_key_value_pair*));
	return table;
}

void ht_delete(ht_hash_table* table)
{
	size_t i = 0;

	for (i = 0; i < table->size; ++i)
	{
		ht_key_value_pair* item = table->items[i];

		if (item != NULL)
			ht_delete_item(item);
	}

	free(table->items);
	free(table);
}

void ht_insert_entry(ht_hash_table* table, const char* key, const char* value)
{
	if ((table->count * 100 / table->size) > 70)
		ht_resize_up(table);

	ht_key_value_pair* item = ht_new_item(key, value);
	int index = ht_get_hash(item->key, table->size, 0);
	ht_key_value_pair* current = table->items[index];
	int i = 1;

	while (current != NULL)
	{
		if (current != &HT_DELETED_ITEM)
		{
			if (strcmp(current->key, key) == 0)
			{
				ht_delete_item(current);
				table->items[index] = item;
				return;
			}
		}

		index = ht_get_hash(item->key, table->size, i);
		current = table->items[index];
		++i;
	}

	table->items[index] = item;
	table->count++;
}

char* ht_search_entry(ht_hash_table* table, const char* key)
{
	int index = ht_get_hash(key, table->size, 0);
	ht_key_value_pair* item = table->items[index];
	int i = 0;

	while (item != NULL)
	{
		if (item != &HT_DELETED_ITEM)
		{
			if (strcmp(item->key, key) == 0)
				return item->value;
		}

		index = ht_get_hash(key, table->size, i);
		item = table->items[index];
		++i;
	}

	return NULL;
}

void ht_delete_entry(ht_hash_table* table, const char* key)
{
	if ((table->count * 100 / table->size) < 10)
		ht_resize_down(table);

	int index = ht_get_hash(key, table->size, 0);
	ht_key_value_pair* item = table->items[index];
	int i = 1;

	while (item != NULL)
	{
		if (item != &HT_DELETED_ITEM)
		{
			if (strcmp(item->key, key) == 0)
			{
				ht_delete_item(item);
				table->items[index] = &HT_DELETED_ITEM;
				table->count--;
			}
		}
		index = ht_get_hash(key, table->size, i);
		item = table->items[index];
		++i;
	}
}

size_t ht_get_size(ht_hash_table* table)
{
	return table->size;
}

size_t ht_get_count(ht_hash_table* table)
{
	return table->count;
}

static void ht_resize(ht_hash_table* table, const size_t size)
{
	if (size < HT_MIN_SIZE) return;

	ht_hash_table* new_table = ht_new(size);

	for (size_t i = 0; i < table->size; ++i)
	{
		ht_key_value_pair* item = table->items[i];
		if (item != NULL && item != &HT_DELETED_ITEM)
		{
			ht_insert_entry(new_table, item->key, item->value);
		}
	}

	ht_delete(table);
	table = new_table;
}

static void ht_resize_up(ht_hash_table* table)
{
	const size_t new_size = (size_t)((double)table->size * 2);
	ht_resize(table, new_size);
}

static void ht_resize_down(ht_hash_table* table)
{
	const size_t new_size = (size_t)((double)table->size / 2);
	ht_resize(table, new_size);
}
