static double factorial(double n)
{
  if (n < 2) return 1;
  return n * factorial(n-1);
}

int rank(int *num, int siz)
{
  int i;

  if (siz < 1) return 0;

  for (i = 1; i < siz; i++)
    if (num[i] > num[0])
      num[i]--;

  return (num[0] - 1) * factorial(siz - 1) + rank(num+1, siz-1);
}
