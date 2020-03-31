#include <stdio.h>
static int numRefs; /* numero de referencias encontradas */
%}
HREF       ([Hh][Rr][Ee][Ff])
ESPACO     [ \n\t]
%s ACCAO COMENTARIO REF
%%
<ACCAO>\>                                BEGIN(INITIAL);
<ACCAO>{HREF}{ESPACO}*={ESPACO}*\"       BEGIN(REF);
<ACCAO>(.|\n)                            ;

<REF>#[^"]*                 ; /* ignorar referencias internas */
<REF>javascript:[^"]*       ; /* ignorar referencias a javascript */
<REF>[a-zA-Z]+:[^"]*        printf("[%d] %s\n",++numRefs,yytext);
<REF>[^"]*                  numRefs++;
<REF>\"                     BEGIN(ACCAO);

<COMENTARIO>--\>            BEGIN(INITIAL);
<COMENTARIO>(.|\n)          ;

\<                          BEGIN(ACCAO);
\<!--                       BEGIN(COMENTARIO);
(.|\n)                      ;
%%
int main (int argc,char *argv[]){
       if (argc > 1) {
           if ((yyin=fopen(argv[1],"rb")) == NULL) {
             perror(argv[1]);
             return 2;
           }
       }
       numRefs=0;

       yylex();

       printf("Foram encontradas %ld referencias em %s.\n",
              numRefs, argv[1] ? argv[1] : "<stdin>");

       return 0;
}

