#include <stdio.h>
#include <math.h>
int main()
{
  int xm, x2m, n, x;

  xm = x2m = n = 0;
  while (scanf("%d", &x) > 0) {
    n++;
    xm += x;
    x2m += x*x;
  }
  printf("%d %g %lg\n", n, xm/((float)n),
         sqrt((x2m - xm*xm/((double)n))/((double)n)));
  return 0;
}
