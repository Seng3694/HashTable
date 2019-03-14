#include <assert.h>
#include <string.h>

#include "prime.h"
#include "hash_table.h"

#define streq(str1, str2) (strcmp(str1, str2) == 0)

int main(void)
{
	ht_hash_table* table = ht_new(50);

	assert(ht_get_count(table) == 0);
	assert(ht_get_size(table) == ht_next_prime(50));

	ht_insert_entry(table, "1", "test1");
	ht_insert_entry(table, "3", "test3");

	assert(ht_get_count(table) == 2);
	assert(ht_get_size(table) == ht_next_prime(50));

	assert(streq(ht_search_entry(table, "1"), "test1"));
	assert(ht_search_entry(table, "2") == NULL);
	assert(streq(ht_search_entry(table, "3"), "test3"));

	ht_delete_entry(table, "1");
	assert(ht_get_count(table) == 1);
	assert(ht_get_size(table) == ht_next_prime(50));
	assert(ht_search_entry(table, "1") == NULL);

	ht_delete_entry(table, "2");
	assert(ht_get_count(table) == 1);
	assert(ht_get_size(table) == ht_next_prime(50));

	ht_delete(table);
	return 0;
}
