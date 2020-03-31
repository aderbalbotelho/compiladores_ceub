#include <stdio.h>
int func(int arg) {
  int *args = &arg;

  printf("%d %d %d\n", args[0], args[1], args[2]);
  return arg + args[0];
}
