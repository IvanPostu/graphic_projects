#ifndef COMPACT_BOOL_H
#define COMPACT_BOOL_H

#include "../utils/utils.h"

typedef unsigned char byte;
STATIC_ASSERT(sizeof(byte) == 1, "byte must have size 1");

typedef byte compact_8b_bool;

typedef enum {
  BI_ZERO = 0,
  BI_ONE,
  BI_TWO,
  BI_THREE,
  BI_FOUR,
  BI_FIVE,
  BI_SIX,
  BI_SEVEN
} bit_index_t;
typedef enum { BIT_ZERO = 0, BIT_ONE } bit_t;

// set true by index
void set_one_compact_8b_bool(compact_8b_bool *b, bit_index_t index);

// set false by index
void set_zero_compact_8b_bool(compact_8b_bool *b, bit_index_t index);

// reverse bit by index
void toggle_compact_8b_bool(compact_8b_bool *b, bit_index_t index);

// read bit by index
bit_t read_compact_8b_bool(compact_8b_bool *b, bit_index_t index);

// returns bits as char array
char *compact_8b_bool_to_char_bits(compact_8b_bool *b);

#endif // COMPACT_BOOL_H
