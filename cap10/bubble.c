int compare(double a[],double b[]);
void bubble(double v[], int n) {
  int i, j;
  double tmp[1];

  for (i = 0; i < n - 1; i = i + 1) {
    for (j = 0; j < n - 1 - i; j = j + 1) {
      if (compare(v+j+1, v+j) > 0) {
        tmp[0] = v[j];
        v[j] = v[j+1];
        v[j+1] = tmp[0];
      }
    }
  }
}
