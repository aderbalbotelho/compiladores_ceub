#include <stdio.h>

int main() {
  union { short val; char vec[2]; } x;
  x.val = 1;
  if (x.vec[0] == 1) printf("little-endian\n");
  if (x.vec[1] == 1) printf("big-endian\n");
  return 0; 
}
