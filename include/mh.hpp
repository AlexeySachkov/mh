#include <stdint.h>
#include <array>
#include <vector>

typedef int64_t int_t;
typedef uint64_t uint_t;
typedef int16_t block_t;

const uint_t BLOCK_SIZE = 16;

typedef std::array<uint_t, BLOCK_SIZE> key_t;
typedef std::vector<block_t> blocks_t;
typedef std::vector<uint_t> numbers_t;

#define GET_BIT(block, bit) (((block) & (((uint_t)1) << (bit))) != 0)
#define CLR_BIT(block, bit) ((block) &= (~(((uint_t)1) << (bit))))
#define SET_BIT(block, bit) ((block) |= (((uint_t)1) << (bit)))
#define UPD_BIT(block, bit, value) ((value) == 0) ? CLR_BIT((block), (bit)) : SET_BIT((block), (bit));

uint_t gcd_extended(const uint_t, const uint_t, uint_t&, uint_t&);

key_t generate_public_key(const key_t&, const uint_t, const uint_t);

uint_t encrypt_block(const block_t, const key_t&);

block_t decrypt_number(const uint_t, const key_t&, const uint_t, const uint_t);