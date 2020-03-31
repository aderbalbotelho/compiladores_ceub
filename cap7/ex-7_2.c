#include <stdio.h>

int x = 12;

void printx() { printf("x=%d\n",x); }

int main() {
  int *z = &x;
  int x = 22;

  printx();
  printf("x=%d\n",x);
  *z = 52;
  printx();

  return 0;
}
