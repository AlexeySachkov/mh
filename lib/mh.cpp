#include "mh.hpp"

uint_t gcd_extended(const uint_t a, const uint_t b, uint_t &x, uint_t &y)
{
	if (a == 0)
	{
		x = 0;
		y = 1;
		return b;
	}
	uint_t x1, y1;
	uint_t d = gcd_extended(b % a, a, x1, y1);
	x = y1 - (b / a) * x1;
	y = x1;
	return d;
}

key_t generate_public_key(const key_t &private_key, const uint_t N, const uint_t M)
{
	key_t result;
	for (uint_t i = 0; i < BLOCK_SIZE; ++i)
	{
		result[i] = (private_key[i] * N) % M;
	}

	return result;
}

uint_t encrypt_block(const block_t block, const key_t &public_key)
{
	uint_t result = 0;

	for (uint_t i = 0; i < BLOCK_SIZE; ++i)
	{
		if (GET_BIT(block, i))
		{
			result += public_key[i];
		}
	}

	return result;
}

block_t decrypt_number(const uint_t encrypted, const key_t &private_key, const uint_t N, const uint_t M)
{
	block_t result;
	uint_t sum = 0;
	uint_t NN, y;
	gcd_extended(N, M, NN, y);

	sum = (encrypted * NN) % M;

	for (int_t i = BLOCK_SIZE - 1; i >= 0 && sum > 0; --i)
	{
		if (sum >= private_key[i])
		{
			sum -= private_key[i];
			SET_BIT(result, i);
		}
	}

	return result;
}