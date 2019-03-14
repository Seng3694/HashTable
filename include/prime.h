#ifndef HT_PRIME_H
#define HT_PRIME_H

typedef enum
{
	HT_IS_PRIME,
	HT_NOT_PRIME,
	HT_UNDEFINED
} HT_PRIME_RESULT;

HT_PRIME_RESULT ht_is_prime(const int x);
int ht_next_prime(int x);

#endif
