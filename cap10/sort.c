void sort(int *vec, int siz)
{
  int i, j, max, tmp;

  for (i = siz; i > 1; i--) {
    max = 1;
    for (j = 2; j < i; j++) // o maior
      if (vec[j] > vec[max])
        max = j;
    if (max != i) { // colocar no lugar
      tmp = vec[i];
      vec[i] = vec[max];
      vec[max] = tmp;
    }
  }
}
