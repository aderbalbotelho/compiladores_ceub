int func(int x, int *y) {
  x += 2;
  *y += 2;
  y += 2;
  return x * *y;
}

int main() {
  int vec[] = { 1, 2, 3, 5, 7, 9, 11, 13, 17 };

  printf("%d\n", func(5, vec));
  printf("%d\n", func(3, vec));
  printf("%d\n", func(1, vec));

  return 0;
}
