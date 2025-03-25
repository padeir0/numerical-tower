#include "number.h"
#include <stdlib.h>
#include <string.h>

#define MIN_NAT_VEC 4
#define DIGITS_PER_INT 9
#define BASE 1_000_000_000

number* alloc_number() {
  return (number*)malloc(sizeof(number));
}

u32* natvec_alloc(int size) {
  return (u32*)malloc(size*sizeof(u32));
}

void natvec_copy(u32* dest, u32* source, int len) {
  memcpy(dest, source, len*sizeof(u32));
}

void natvec_free(u32* vec) {
  free(vec);
}

_natural natural_empty() {
  _natural n;
  n.cap = 0;
  n.len = 0;
  n.digits = NULL;
  return n;
}

bool natural_push_digit(_natural* out, u32 digit) {
  if (out->cap == 0) {
    out->digits = natvec_alloc(MIN_NAT_VEC);
    if (out->digits == NULL) {
      return false;
    }
    out->cap = MIN_NAT_VEC;
  }
  if (out->len == out->cap) {
    i32 new_cap = 2 * out->cap;
    u32* new_vec = natvec_alloc(new_cap);
    if (new_vec == NULL) {
      return false;
    }
    natvec_copy(new_vec, out->digits, out->len);
    natvec_free(out->digits);
    out->digits = new_vec;
    out->cap = new_cap;
  }
  int index = out->len;
  out->len++;
  out->digits[index] = digit;
  return true;
}

int natural_write_u32(u32 n, char* buffer, bool first) {
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

  int distance = (uptr)b - (uptr)buffer;
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
    u32 curr_digit = nat.digits[i];
    natural_write_u32(curr_digit, block, first);
    first = false;
    block += DIGITS_PER_INT;
    i--;
  }

  int size = (uptr)block - (uptr)buffer;
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

number* number_new_int(i32 integer) {
  number* num = alloc_number();
  num->tag = nk_INTEGER;

  i8 sign = 1;
  if (integer < 0) {
    sign = -1;
    integer = -integer;
  }

  natural_push_digit(&num->data.integer.abs, integer);
  return num;
}
