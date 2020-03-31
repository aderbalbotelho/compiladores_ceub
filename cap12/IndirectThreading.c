#define STKSIZ 20000
long stack[STKSIZ], *sp = stack;
typedef void **F;
F *ip;
void interp()
{
  static void *Integer_ = &&Integer, *Add_ = &&Add, *Exit_ = &&Exit;
  F codigo[] = { &&Integer_, (F)3, &&Integer_, (F)5, &&Add_,  &&Exit_ };

#define NEXT goto ***ip++
  ip = codigo;
  NEXT;
  Integer: *sp++ = (long)*ip++; NEXT;
  Add: sp--; sp[-1] += sp[0]; NEXT;
  Exit: printf("%d\n", sp[-1]); return;
}

