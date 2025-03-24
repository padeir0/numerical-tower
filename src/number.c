#include "number.h"
#include <stdlib.h>
#include <string.h>

#define MIN_NAT_VEC 4
#define DIGITS_PER_INT 9
#define BASE 1_000_000_000

uint32_t* alloc_natvec(int size) {
  return (uint32_t*)malloc(size*sizeof(uint32_t));
}

number* alloc_number() {
  return (number*)malloc(sizeof(number));
}

void natvec_copy(uint32_t* dest, uint32_t* source, int len) {
  memcpy(dest, source, len*sizeof(uint32_t));
}

void natural_set(_natural* nat, uint32_t integer) {
  nat->len = 1;
  nat->digits[0] = integer;
}

_natural natural_empty() {
  _natural n;
  n.cap = 0;
  n.len = 0;
  n.digits = NULL;
  return n;
}

void natural_push_digit(_natural* out, uint32_t digit) {
  if (out->cap == 0) {
    out->digits = alloc_natvec(MIN_NAT_VEC);
    out->cap = MIN_NAT_VEC;
  }
  if (out->len == out->cap) {
    int32_t new_cap = 2 * out->cap;
    uint32_t* new_vec = alloc_natvec(new_cap);
    natvec_copy(new_vec, out->digits, out->len);
    out->digits = new_vec;
    out->cap = new_cap;
  }
  int index = out->len;
  out->len++;
  out->digits[index] = digit;
}

int natural_write_u32(uint32_t n, char* buffer, bool first) {
  if (n == 0) {
    int i = 0;
    while (i < DIGITS_PER_INT) {
      buffer[i] = '0';
      i++;
    }
    return DIGITS_PER_INT;
  }

  char* b = buffer + DIGITS_PER_INT -1;
  while (n > 0) {
    *b = (char)(n%10) + '0';
    b--;
    n = n/10;
  }
  if (!first) {
    while (b >= buffer) {
  	  *b = '0';
      b -= 1;
    }
  }

  int distance = (uintptr_t)b - (uintptr_t)buffer;
  return 9 - distance;
}

int natural_i_buffwrite(_natural nat, char* buffer, int bufflen, bool pad_left, bool pad_right) {
  // Checks if the buffer size is sufficient, we're generous here and
  // don't care if padding is omitted. We also expect that no useless
  // digits are present, ie: 000000000_000000001,
  // which should be true for all arithmetic implemented here.
  if (bufflen == 0 || nat.len * DIGITS_PER_INT >= bufflen) {
    return 0;
  }
  if (nat.len == 0) {
    *buffer = '0';
    return 1;
  }

  int i = nat.len -1;
  char* block = buffer;
  bool first = !pad_left;

  while (0 <= i) {
    uint32_t curr_digit = nat.digits[i];
    natural_write_u32(curr_digit, block, first);
    first = false;
    block += DIGITS_PER_INT;
    i--;
  }

  int size = (uintptr_t)block - (uintptr_t)buffer;
  if (!pad_right) {
    while (0 < size && *(buffer+size-1) == '0') {
        size--;
    }
  }

  return size;
}

int natural_buffwrite(_natural nat, char* buffer, int bufflen) {
  return natural_i_buffwrite(nat, buffer, bufflen, false, true);
}

number* number_new_int(int32_t integer) {
  number* num = alloc_number();
  num->tag = nk_INTEGER;

  int8_t sign = 1;
  if (integer < 0) {
    sign = -1;
    integer = -integer;
  }
  uint32_t* vec = alloc_natvec(MIN_NAT_VEC);
  num->data.integer.sign = sign;
  num->data.integer.abs.digits = vec;
  num->data.integer.abs.cap = MIN_NAT_VEC;
  natural_set(&(num->data.integer.abs), (uint32_t) integer);
  return num;
}
