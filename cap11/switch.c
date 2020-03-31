#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int cmp(int *a, int *b) { return *a - *b; }

int gen(int v[], int b[], int lb, int ub);
int main(int argc, char *argv[])
{
  int opts[1000], nopt = 0;
  int buckets[1000], k, n;
  int min, max, tmp, i;
  char *ptr, *str;
  float density = 0.5;

  if (argc < 2) {
    fprintf(stderr, "USAGE: [-d density] %s (case|range|list)+\n"
    		"\tbucket density s a real number (0 < density < 1)\n"
    		"\tcase is an integer literal\n"
    		"\trange is defined by two ':' separated integer literals\n"
    		"\tlist is a sequence of ',' separated integer literals\n",
    		argv[0]);
    return 1;
  }
  if (strcmp(argv[1], "-d") == 0 && argc > 3) {
    density = atof(argv[2]);
    argv += 2;
  }
  while (*++argv)
    if ((ptr = strchr(argv[0], ':')) != 0) {
      *ptr++ = 0;
      printf("range from %s to %s.\n", argv[0], ptr);
      i = opts[nopt++] = atoi(argv[0]);
      tmp = atoi(ptr);
      while (++i <= tmp) opts[nopt++] = i;
    } else if ((ptr = strchr(argv[0], ',')) == 0) {
      printf("case of %s.\n", argv[0]);
      opts[nopt++] = atoi(argv[0]);
    } else {
      *ptr++ = 0;
      printf("list of %s", argv[0]);
      opts[nopt++] = atoi(argv[0]);
      for (str = ptr; (ptr = strchr(str, ',')) != 0; str = ptr) {
        *ptr++ = 0;
	printf(", %s", str);
	opts[nopt++] = atoi(str);
      }
      printf(", %s.\n", str);
      opts[nopt++] = atoi(str);
    }

  max = min = opts[0];
  for (i = 0; i < nopt; i++) {
    if (max < opts[i]) max = opts[i];
    if (min > opts[i]) min = opts[i];
  }

  qsort(opts, nopt, sizeof(int), (int(*)(const void *, const void *))cmp);

  for (i = 0; i < nopt; i++)
    printf(" %d", opts[i]);
  printf("-> [%d, %d] #%d => %d%%\n", min, max, nopt, 100*nopt/(max - min + 1));

  for (n = k = 0; k < nopt; k++, n++) {
    /* first case index in the new bucket */
    buckets[n] = k;
/* determine the density of merging this bucket with the previous one */
#define den(i,j) ((j-buckets[i]+1.0)/(opts[j]-opts[buckets[i]]+1))
/* while density is not below threshold, merge buckets (from last to first) */
     while (n > 0 && den(n-1, k) >= density)
	n--; /* throw away the n-th bucket (this bucket is now the previous) */
  }
  /* this is not a bucket, just a marker to determine the last case index
   * in the previous bucket (which is 'bucket[n]-1') */
  buckets[n] = nopt;

  /* print first case index (not value) of each bucket */
  printf("%d buckets:", n);
  for (i = 0; i < n; i++)
    printf(" %d->%d", opts[buckets[i]], opts[buckets[i+1]-1]);
  printf("\n");

  printf("; switch ( val )\n; L_0 is default\n");
  gen(opts, buckets, 0, n-1);

  return 0;
}

int lblno;
int gen(int v[], int b[], int lb, int ub)
{
  int i, l, u, k = (lb + ub)/2;
  int deflab = 0, lolab = deflab, hilab = deflab;

  if (k > lb) lolab = ++lblno;
  if (k < ub) hilab = ++lblno;
  l = b[k];
  u = b[k+1] - 1;
  if (u - l + 1 <= 3) { /* if sequence for buckets with upto 3 cases */
    for (i = l; i <= u; i++)
      printf("if (val == %d) goto case_%d\n", v[i], v[i]);
    if (k > lb)
      printf("if (val > %d) goto L_%d\n", v[u], hilab);
    else if (k < ub)
      printf("if (val < %d) goto L_%d\n", v[l], lolab);
    else printf("goto L_%d\n", lolab);
  } else { /* table jmp for larger (4+ cases) buckets */
    printf("if (val < %d) goto L_%d\n", v[l], lolab);
    printf("if (val > %d) goto L_%d\n", v[u], hilab);
    printf("jmp table[(val - %d) * 4]\n", v[l]);
    printf("; jmp [(table - %d) + val * 4]\n", v[l]*4);
    for (i = v[l]; i <= v[u]; i++)
      if (v[l] == i)
	printf("\tgoto L_%d\t; %d\n", v[l++], i);
      else
	printf("\tgoto L_%d\t; %d\n", deflab, i);
  }
  if (k > lb) {
    printf("L_%d:\n", lolab);
    gen(v, b, lb, k - 1);
  }
  if (k < ub) {
    printf("L_%d:\n", hilab);
    gen(v, b, k + 1, ub);
  }
}
