double polinomio(double x, int sz, double pol[])
{
  int i;
  double ret = pol[0];

  for (i = 1; i < sz; i++) {
    ret += pol[i] * x;
    x *= x;
  }
  return ret;
}
