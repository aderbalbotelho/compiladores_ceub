#include <stdio.h>

int main() {
  double a = 1.1, b = 0.8, c = 0.125, e = 0.125E5;
  long long *d;
  printf("%x %x %x %x %x\n", 12, 848123, -1, -12, -848124);
  printf("%x %x %x %x %x\n", 12, 848123, ~1, ~12, ~848124);
  printf("%x %x %x %x %x\n", 12+124, 848123+124, -1+124, -12+124, -848124+124);
  d = (long long*)&a;
  printf("%llx\n", *d);
  d = (long long*)&b;
  printf("%llx\n", *d);
  d = (long long*)&c;
  printf("%llx\n", *d);
  d = (long long*)&e;
  printf("%llx\n", *d);
  return 0;
}
