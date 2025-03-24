#include "../src/number.h"
#include <stdio.h>

int main() {
  printf("sizeof(number)   = %lu bytes\n", sizeof(number));
  printf("sizeof(_natural) = %lu bytes\n", sizeof(_natural));
  printf("sizeof(integer)  = %lu bytes\n", sizeof(integer));
  printf("sizeof(rational) = %lu bytes\n", sizeof(rational));
  printf("sizeof(decimal)  = %lu bytes\n", sizeof(decimal));
  printf("\n");
  printf("sizeof(number_kind)   = %lu bytes\n", sizeof(enum number_kind));
  return 0;
}
