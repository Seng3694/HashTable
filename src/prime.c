#include <math.h>

#include "prime.h"

HT_PRIME_RESULT ht_is_prime(const int x)
{
	if (x < 2) return HT_UNDEFINED;
	if (x < 4) return HT_IS_PRIME;
	if ((x % 2) == 0) return HT_NOT_PRIME;

	for (int i = 3; i <= floor(sqrt((double)x)); i += 2)
	{
		if (x % i == 0) 
			return HT_NOT_PRIME;
	}

	return HT_IS_PRIME;
}

int ht_next_prime(int x)
{
	while (ht_is_prime(x) != HT_IS_PRIME)
	{
		x++;
	}

	return x;
}
