grammar simple;

options {
       language = C;
       k = 2; // for PRINT instruction
}

tokens {
       UMINUS;
       JMP;
       LABEL;
       JNZ;
       JZ;
}

@header {
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "node.h"
#include "tabid.h"

}
@members {
  FILE *yyin;
  int yylineno;
  int yydebug;
#ifdef YYDEBUG
char **yynames=(char**)simpleParserTokenNames;
#endif

#include "simpleLexer.h"
  static pANTLR3_INPUT_STREAM          input;
  static psimpleLexer                  lxr;
  static pANTLR3_COMMON_TOKEN_STREAM   tstrm;
  static psimpleParser                 psr;

  int yyopen(char *filename) {
      input = antlr3AsciiFileStreamNew((pANTLR3_UINT8)filename);
      if (input == NULL) { perror(filename); return 0; }
      lxr = simpleLexerNew(input);
      if (lxr == NULL) { fprintf(stderr, "No memory (lexer)\n"); return 0; }
      tstrm = antlr3CommonTokenStreamSourceNew(ANTLR3_SIZE_HINT, TOKENSOURCE(lxr));
      if (tstrm == NULL)  { fprintf(stderr, "No memory (tstream)\n"); return 0; }
      psr = simpleParserNew(tstrm);
      if (psr == NULL)  { fprintf(stderr, "No memory (parser)\n"); return 0; }
      return 1;
  }

  int yyparse() {
    Node *tree;
    tree = psr->start(psr); /* nome da regra principal */
    evaluate(tree);

    psr->free  (psr);           psr = NULL;
    tstrm ->free  (tstrm);      tstrm = NULL;
    lxr->free  (lxr);           lxr = NULL;
    input->close (input);       input = NULL;

    return 0;
  }
}


SPACE:       ( ' ' | '\t' | '\r' '\n' | '\r' | '\n' ) { $channel=HIDDEN; };
COMMENT:     '#' (~('\n'|'\r'))* { $channel=HIDDEN; };
STRING:      '\'' ( ~'\'' )* '\'';
PRINT:       'print';
READ:        'read';
WHILE:       'while';
IF:          'if';
ELSE:        'else';
VARIABLE:    ('a'..'z'|'A'..'Z'|'_') ('a'..'z'|'A'..'Z'|'_'|'0'..'9')* ;
INTEGER:     ('0'..'9')+ ;
GE:          '>=';
LE:          '<=';
NE:          '!=';
EQ:          '==';

var returns [Node *n]
       : v=VARIABLE       { yylineno = $v.line; $n = strNode(VARIABLE, $v.text->chars); }
       ;

start returns [Node *tree]: 'program' list 'end' EOF { $tree = $list.n; };

stmt returns [Node *n]
       : ';'                         { $n = 0; }
       | PRINT s=STRING ';'          { char *str = $s.text->chars;
                                       str[strlen(str)-1] = 0;
                                       $n = strNode(STRING, str+1); }
       | PRINT expr ';'              { $n = subNode(PRINT, 1, $expr.n); }
       | READ var ';'                { $n = strNode(READ, $var.n->value.s); }
       | var '=' expr ';'            { $n = subNode('=', 2, $var.n, $expr.n);}
       | WHILE '(' expr ')' w=stmt   { $n = subNode(WHILE, 2, $expr.n, $w.n); }
       | IF '(' expr ')' i=stmt
           ( ( ELSE ) => ELSE e=stmt { $n = subNode(IF, 3, $expr.n, $i.n, $e.n); }
           | ( )                     { $n = subNode(IF, 2, $expr.n, $i.n); }
           )
       | '{' list '}'                { $n = $list.n; }
       ;

list returns [Node *n]
       : ( stmt {
              if ($n == NULL) $n = $stmt.n;
              else $n = subNode(';', 2, $n, $stmt.n);
              // $n.append($stmt.n);
              } )*
       ;

bexpr returns [Node *n]
       : i=INTEGER                    { $n = intNode(INTEGER, atoi($i.text->chars)); }
       | var                          { $n = $var.n; }
       | '(' expr ')'                 { $n = $expr.n; }
       ;

nexpr returns [Node *n]
       : '-' bexpr                    { $n =  subNode(UMINUS, 1, $bexpr.n); }
       | bexpr                        { $n = $bexpr.n; }
       ;

mexpr returns [Node *n]
       : ne=nexpr                     { $n = $ne.n; }
              ( '*' ne2=nexpr         { $n = subNode('*', 2, $n, $ne2.n); }
              | '/' ne3=nexpr         { $n = subNode('/', 2, $n, $ne3.n); }
              | '\%' ne4=nexpr        { $n = subNode('\045', 2, $n, $ne4.n); }
              )*
       ;

sexpr returns [Node *n]
       : me=mexpr                     { $n = $me.n; }
              ( '+' me2=mexpr         { $n = subNode('+', 2, $n, $me2.n); }
              | '-' me3=mexpr         { $n = subNode('-', 2, $n, $me3.n); }
              )*
       ;

expr returns [Node *n]
       : se=sexpr                     { $n = $se.n; }
              ( '<' se2=sexpr         { $n = subNode('<', 2, $n, $se2.n); }
              | '>' se3=sexpr         { $n = subNode('>', 2, $n, $se3.n); }
              | GE  se4=sexpr         { $n = subNode(GE, 2, $n, $se4.n); }
              | LE  se5=sexpr         { $n = subNode(LE, 2, $n, $se5.n); }
              | NE  se6=sexpr         { $n = subNode(NE, 2, $n, $se6.n); }
              | EQ  se7=sexpr         { $n = subNode(EQ, 2, $n, $se7.n); }
              )*
       ;
