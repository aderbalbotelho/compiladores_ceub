int a() { printf("a()\n"); return 1; }
int b() { printf("b()\n"); return 2; }
int c() { printf("c()\n"); return 3; }
int d() { printf("d()\n"); return 4; }
int f(int x, int y) {
  printf("f()\n");
  return y - x;
}
int main() {
  printf("%d\n", f(a() + b(), c() + d()));
  return 0;
}
