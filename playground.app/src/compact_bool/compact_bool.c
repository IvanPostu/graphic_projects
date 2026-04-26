#include "compact_bool.h"
#include <stdlib.h>

void set_one_compact_8b_bool(compact_8b_bool *b, bit_index_t index) {
  *b = *b | (1 << index);
}

void set_zero_compact_8b_bool(compact_8b_bool *b, bit_index_t index) {
  compact_8b_bool mask = ~(1 << index);
  *b = *b & mask;
}

void toggle_compact_8b_bool(compact_8b_bool *b, bit_index_t index) {
  compact_8b_bool mask = 1 << index;
  *b = *b ^ mask;
}

bit_t read_compact_8b_bool(compact_8b_bool *b, bit_index_t index) {
  compact_8b_bool mask = 1 << index;
  bit_t result = *b & mask;
  return result ? 1 : 0;
}

char *compact_8b_bool_to_char_bits(compact_8b_bool *b) {
  char *bits = malloc(9);
  if (!bits)
    return NULL;

  for (int i = 7; i >= 0; i--) {
    int mask = (1 << i);
    bits[7 - i] = (*b & mask) ? '1' : '0';
  }
  bits[8] = '\0';
  return bits;
}
