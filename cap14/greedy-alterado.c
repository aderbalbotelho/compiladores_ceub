/* $Id: greedy.c,v 1.0 2012/01/12 17:25:13 prs Exp $ */
/* half-way solution between postfix.c and compact.brg
 * eval returns the register (and possibly other data)
 * and emits directly assembler (without macros)
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "node.h"
#include "y.tab.h"
#include "tabid.h"

#define ACC "eax"

static int lbl;
static FILE *out;
static char busy[7];
static int eval(Node *p), getReg();
static char *mklbl(int), *mkreg(int);

static int data(int t, char *s, int a, int user) {
  if (s)
    printf(" %s dd 0\n", s);
  return 1;
}

void evaluate(Node *p) {
  extern char *outfile, **yynames;
  extern int errors, tree;

  if (errors > 0) return;
  if (tree > 0) { printNode(p, stdout, yynames); return; }
  if (outfile == 0) outfile = "out.asm";
  if ((out = fopen(outfile, "w")) == 0) {
    perror(outfile);
    exit(2);
  }
#ifdef _WIN32
  fprintf(out," segment .text\n align 4\n global _main\n_main:\n");
#else
  fprintf(out," segment .text\n align 4\n global _main:function\n_main:\n");
#endif
  eval(p);
  fprintf(out," mov " ACC ", 0\n ret\n");
  fprintf(out," segment .data\n");
  IDevery(data,0);
  fprintf(out," extern _prints\n extern _printi\n extern _println\n extern _readi\n");
  fclose(out);
}


static int eval(Node *p) {
    int i, lbl1, lbl2, r1, reg = 0;
    char *name;

    if (p == 0) return 0;
    switch(p->attrib) {
    case INTEGER:
        fprintf(out, " mov %s, %d\n", mkreg(reg = getReg()), p->value.i);
        break;
    case STRING:
	/* generate the string */
	fprintf(out, ".segment .rodata\nalign 4\n%s: \"%s\"\n.segment .text\n",
	    	mklbl(lbl1 = ++lbl), p->value.s);
	/* make the call */
	fprintf(out, " push dword %s\n call _prints\n call _println\n add esp, 4\n",  mklbl(lbl1));
        break;
    case VARIABLE:
	fprintf(out, " mov %s, dword [%s]\n", mkreg(reg = getReg()), p->value.s);
	break;
    case WHILE:
	fprintf(out, "%s:\n", mklbl(lbl1 = ++lbl));
	eval(p->SUB(0));
	fprintf(out, " jz %s\n", mklbl(lbl2 = ++lbl));
	eval(p->SUB(1));
	fprintf(out, " jmp %s\n", mklbl(lbl1));
	fprintf(out, "%s:\n", mklbl(lbl2));
	break;
    case IF:
	eval(p->SUB(0));
	fprintf(out, " jz %s\n", mklbl(lbl1 = ++lbl));
	eval(p->SUB(1));
	if (p->value.sub.num > 2) { /* if else */
	    fprintf(out, " jmp %s\n", mklbl(lbl2 = ++lbl));
	    fprintf(out, "%s:\n", mklbl(lbl1));
	    eval(p->SUB(2));
	    lbl1 = lbl2;
	}
	fprintf(out, "%s:\n", mklbl(lbl1));
	break;
    case READ:
	fprintf(out, " call _readi\n mov [%s], " ACC "\n", p->value.s);
	break;
    case PRINT:
	eval(p->SUB(0));			/* determine the value        */
	fprintf(out, " call _printi\n call _println\n add " ACC ", 4\n");
	break;
    case ';':
	for (i = 0; i < p->value.sub.num; i++)
	  eval(p->SUB(i));
	break;
    case '=':
	fprintf(out, " mov [%s], %s\n", p->SUB(0)->value.s, mkreg(r1 = eval(p->SUB(1))));
	break;
    case UMINUS:
	reg = eval(p->SUB(0));
	fprintf(out, " neg %s\n", mkreg(reg));
	break;
    default:
	reg = eval(p->SUB(0));			/* evaluate first argument    */
	r1 = eval(p->SUB(1));			/* evaluate second argument   */
	switch(p->attrib) {			/* make the operation ...     */
	case '+':   fprintf(out, " add %s, %s", mkreg(reg), mkreg(r1)); break;
	case '-':   fprintf(out, " sub %s, %s", mkreg(reg), mkreg(r1)); break;
	/* all operations below have restrictions not yet addressed */
	case '*':   fprintf(out, " imul %s, %s", mkreg(reg), mkreg(r1)); break;
	case '/':   fprintf(out, " cdq\n div %s, %s", mkreg(reg), mkreg(r1)); break;
	case '%':   fprintf(out, " cdq\n mod %s, %s", mkreg(reg), mkreg(r1)); break;
	case '<':   fprintf(out, " cmp %s, %s\n setl al\n and eax, 1\n", mkreg(reg), mkreg(r1)); break;
	case '>':   fprintf(out, " gt %s, %s\n setg al\n and eax, 1\n", mkreg(reg), mkreg(r1)); break;
	case GE:    fprintf(out, " ge %s, %s\n setge al\n and eax, 1\n", mkreg(reg), mkreg(r1)); break;
	case LE:    fprintf(out, " le %s, %s\n setle al\n and eax, 1\n", mkreg(reg), mkreg(r1)); break;
	case NE:    fprintf(out, " ne %s, %s\n setne al\n and eax, 1\n", mkreg(reg), mkreg(r1)); break;
	case EQ:    fprintf(out, " eq %s, %s\n sete al\n and eax, 1\n", mkreg(reg), mkreg(r1)); break;
	default:    printf("unknown %d\n", p->attrib);
	}
    }
    return reg;
}

static int getReg()
{
  int i;

  for(i = 0; i < sizeof(busy); i++)
    if (busy[i] == 0) {
      busy[i] = 1;
      return i + 1;
    }
  fprintf(stderr,"Registros esgotados\n");
  return 0;
}

static char *mkreg(int n) {
  static char *regs[] = { "error", "eax", "ecx", "edx", "ebx", "esi", "edi" };
  if (n < 0 || n >= sizeof(regs)/sizeof(regs[0])) n = 0;
  return regs[n];
}

static char *mklbl(int n) {
  static char str[20];
  if (n < 0)
    sprintf(str, ".L%d", -n);
  else
    sprintf(str, "_#L%d", n);
  return str;
}
