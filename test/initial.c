// this is just for some initial testing, any
// tests of internal structure would halt development
#include "../src/number.c"
#include <stdio.h>

#define BUFF_LENGTH 2048
char buffer[BUFF_LENGTH];

int main() {
  _natural nat = natural_empty();

  natural_push_digit(&nat, 419716000);
  natural_push_digit(&nat, 327950288);
  natural_push_digit(&nat, 846264338);
  natural_push_digit(&nat, 358979323);
  natural_push_digit(&nat, 314159265);
  
  int written = natural_buffwrite(nat, buffer, BUFF_LENGTH);
  if (written == 0) {
    printf("well...");
    return 1;
  }
  printf("%.*s\n", written, buffer);
  printf("length: %d, cap: %d\n", nat.len, nat.cap);
  return 0;
}
