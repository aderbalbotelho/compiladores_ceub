#include <stdio.h>

static int x = 12;
extern int y;

int func(int y) {
    static int x;

    printf("x=%d\n", x);
    return x += y;
}

int main() {
  func(5);
  func(x);
  func(y);

  return 0;
}
