void Integer() { *sp++ = (long)*ip++;; }
void Add() { sp--; sp[-1] += sp[0]; }
void Exit() { printf("%d\n", sp[-1]); exit(0); }

#define STKSIZ 20000
long stack[STKSIZ], *sp = stack;
typedef void (*F)(void);
F *ip;
void interp()
{
  F codigo[] = { Integer, (F)3, Integer, (F)5, Add, Exit };
  
  ip = codigo;
  while (1)
    (*ip++)();
}
