#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

enum __attribute__((__packed__)) number_kind {
  nk_INTEGER,
  nk_RATIONAL,
  nk_DECIMAL,
};

typedef struct {
  uint32_t* digits;
  int32_t   cap;
  int32_t   len;
} _natural;

typedef struct {
  _natural abs;
  int8_t   sign;
} integer;

typedef struct {
  _natural numerator;
  _natural denominator;
  int8_t   sign;
} rational;

typedef struct {
  _natural coefficient;
  int32_t  exponent;    // max 2^31 digits
  int8_t   sign;
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
  int32_t precision;
  enum number_status status;
  // scratch space
  number* a; number* b; number* c;
} number_context;

number* number_new_int(int32_t integer);
number* number_new_rat(int32_t num, int32_t den);
number* number_new_dec(int32_t integer, int32_t decimal);

void number_add(number_context* ctx, const number* a, const number* b, number* out);
void number_sub(number_context* ctx, const number* a, const number* b, number* out);
void number_mul(number_context* ctx, const number* a, const number* b, number* out);
void number_div(number_context* ctx, const number* a, const number* b, number* quo, number* rem);
void number_neg(number_context* ctx, const number* a, number* out);
void number_abs(number_context* ctx, const number* a, number* out);
