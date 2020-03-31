%{
/*  */
#include <stdio.h>
int nblk;
%}
%x COM COMG COMC GRAM CODE
%%
.|\n ECHO;
"%type".* ;
^"%%" ECHO; BEGIN GRAM;
"/*" BEGIN COM; ECHO;
<GRAM>\'[{}]\' ECHO;
<GRAM>\{ nblk++; BEGIN CODE;
<GRAM>.|\n ECHO;
<GRAM>"/*" BEGIN COMG; ECHO;
<GRAM>^"%%" BEGIN 0; ECHO;

<COMG>"*/" BEGIN GRAM; ECHO;
<COMG>.|\n ECHO;

<COM>"*/" BEGIN 0; ECHO;
<COM>.|\n ECHO;

<CODE>\{ nblk++;
<CODE>\} if (--nblk == 0) BEGIN GRAM;
<CODE>\"([^"\\]|\\(.|\n))*\" ;
<CODE>\'(.|\\[^']+)\' ;
<CODE>"/*" BEGIN COMC;
<CODE>.|\n ;

<COMC>"*/" BEGIN CODE;
<COMC>.|\n ;
%%
int yywrap() { return 1; }
int main(int argc, char *argv[]) {
  if (argc > 1) yyin = fopen(argv[1], "r");
  while (yylex()) ;
  return 0;
}
