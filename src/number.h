#include "int_sizes.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

enum __attribute__((__packed__)) number_kind {
  nk_INTEGER,
  nk_RATIONAL,
  nk_DECIMAL,
};

typedef struct {
  u32* digits;
  i32  cap;
  i32  len;
} _natural;

typedef struct {
  _natural abs;
  i8       sign;
} integer;

typedef struct {
  _natural numerator;
  _natural denominator;
  i8       sign;
} rational;

typedef struct {
  _natural coefficient;
  i32  exponent;    // max 2^31 digits
  i8   sign;
} decimal;

typedef struct {
  union {
    integer  integer;
    rational rational;
    decimal  decimal;
  } data;
  enum number_kind tag;
} number;

enum __attribute__((__packed__)) number_status {
  ns_OK,
  ns_DIVISION_BY_ZERO,
  ns_OUT_OF_MEMORY,
};

typedef struct {
  i32 precision;
  enum number_status status;
  // scratch space
  number* a; number* b; number* c;
} number_context;

number* number_new_int(i32 integer);
number* number_new_rat(i32 num, i32 den);
number* number_new_dec(i32 integer, i32 decimal);

void number_add(number_context* ctx, const number* a, const number* b, number* out);
void number_sub(number_context* ctx, const number* a, const number* b, number* out);
void number_mul(number_context* ctx, const number* a, const number* b, number* out);
void number_div(number_context* ctx, const number* a, const number* b, number* quo, number* rem);
void number_neg(number_context* ctx, const number* a, number* out);
void number_abs(number_context* ctx, const number* a, number* out);
