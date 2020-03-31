#include <stdio.h>

struct position { int line, column; } pos = { 12, 7 }, loc = { 9, 5 }, *ptr = &pos;
struct point { int x, y, z; } pt1 = { 3, 5, 7 }, *ptp;

int main() {
  ptp = (struct point*)ptr;
  printf("%d %d %d\n", ptr[1].column, ptp->z, ptp[1].y);
  return 0;
}
