int *sort(int *data, int count, int(*compare)(int,int))
{
  register int a, b;
  void *t;

  for (a = 1; a < count; a++) {
    t = data[a];
    b = a - 1;
    while (b >= 0 && (*compare)(t, data[b]) < 0) {
      data[b+1] = data[b];
      b--;
    }
    data[b+1] = t;
  }
  return data;
}

