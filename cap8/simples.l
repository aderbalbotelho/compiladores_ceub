%{ 
/* $Id: simples.l,v 1.7 2004/02/15 01:13:32 prs Exp $ */
#include <stdlib.h> 
#include <string.h> 
#include "node.h" 
#include "y.tab.h" 
char *dupstr(const char *s);
extern void yyerror(const char *msg);
%} 
 
%% 
"#".*			; /* comment */
">="			return GE; 
"<="			return LE; 
"=="			return EQ; 
"!="			return NE; 
"while"			return WHILE; 
"if"			return IF; 
"else"			return ELSE; 
"print"			return PRINT; 
"read"			return READ; 
"program"		return PROGRAM; 
"end"			return END; 

[A-Za-z][A-Za-z0-9_]*	{ yylval.s = dupstr(yytext); return VARIABLE; } 
\'[^']*\'		{ yytext[yyleng-1] = 0; yylval.s = dupstr(yytext+1); return STRING; }
[0-9]+			{ yylval.i = strtol(yytext, 0, 10); return INTEGER; } 
[-()<>=+*/%;{}.]	return *yytext; 
 
[ \t\n\r]+		; /* ignore whitespace */ 
 
.			yyerror("Unknown character"); 
%% 
int yywrap(void) { return 1; } 
char *getyytext() { return yytext; }
