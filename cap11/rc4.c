struct rc4 { unsigned char box[256]; int i, j; };

void rc4(struct rc4 *rc4, char *data, int count)
{
  int swap, paws, i = rc4->i, j = rc4->j;
  while (count-- > 0) {
    i = (i + 1) & 255;
    j = (j + (swap = rc4->box[i])) & 255;
    rc4->box[i] = paws = rc4->box[j];
    rc4->box[j] = swap;
    *data++ ^= rc4->box[(paws + swap) & 255];
  }
  rc4->i = i;
  rc4->j = j;
}








