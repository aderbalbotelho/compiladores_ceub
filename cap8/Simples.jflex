import java.io.*;
%%
// %standalone // use 'lex' with no references to 'yylval'
// %debug
%byaccj
%full // 8bits characters
%line // yyline for counting input lines

%{
  private Bparser parse; // reference to byaccj generated syntax analyser
  public Yylex(Reader r, Bparser p) { this(r); parse = p; } // constructor
  public int line() { return yyline; } // current line number
%}

// start conditions and macros go here 
%% 
"#".*			{ ; /* comment */ }
">="			{ return parse.GE; }
"<="			{ return parse.LE; }
"=="			{ return parse.EQ; }
"!="			{ return parse.NE; }
"while"			{ return parse.WHILE; }
"if"			{ return parse.IF; }
"else"			{ return parse.ELSE; }
"print"			{ return parse.PRINT; }
"read"			{ return parse.READ; }
"program"		{ return parse.PROGRAM; }
"end"			{ return parse.END; }

[A-Za-z][A-Za-z0-9_]*	{ parse.yylval = new NodeVariable(yytext());
			  return parse.VARIABLE; } 
\'[^']*\'		{ parse.yylval = new NodeString(yytext().substring(1, yytext().length()-1));
			  return parse.STRING; }
[0-9]+			{ parse.yylval = new NodeInteger(Integer.parseInt(yytext()));
			  return parse.INTEGER; } 
[-()<>=+*/%;{}.]	{ return (int)yycharat(0); }
 
[ \t\n]+		{ ; /* ignore whitespace */ }
 
.			{ parse.yyerror("Unknown character"); }
