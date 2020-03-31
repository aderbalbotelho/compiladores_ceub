%{
import java.io.*;
%}
%token INTEGER
%token VARIABLE STRING
%token WHILE IF PRINT READ PROGRAM END
%nonassoc IFX
%nonassoc ELSE

%left GE LE EQ NE '>' '<'
%left '+' '-'
%left '*' '/' '%'
%nonassoc UMINUS

%%

program	: PROGRAM list END		  { tree = $2; }
	;

stmt	: ';'				  { $$ = new Node(); }
	| PRINT STRING ';'		  { $$ = $2; }
	| PRINT expr ';'		  { $$ = new NodePrint($2); }
	| READ VARIABLE ';'		  { $$ = new NodeRead($2); }
	| VARIABLE '=' expr ';'		  { $$ = new NodeAssign($1, $3);}
	| WHILE '(' expr ')' stmt	  { $$ = new NodeWhile($3, $5); }
	| IF '(' expr ')' stmt %prec IFX  { $$ = new NodeIf($3, $5); }
	| IF '(' expr ')' stmt ELSE stmt  { $$ = new NodeIfelse($3, $5, $7); }
	| '{' list '}'			  { $$ = $2; }
	;

list	: stmt				  { NodeList n = new NodeList();
					    n.append($1); $$ = n; }
	| list stmt			  { ((NodeList)$1).append($2);
					    $$ = $1; }
	;

expr	: INTEGER			  { $$ = $1; }
	| VARIABLE			  { $$ = $1; }
	| '-' expr %prec UMINUS		  { $$ = new NodeUminus($2); }
	| expr '+' expr			  { $$ = new NodeAdd($1, $3); }
	| expr '-' expr			  { $$ = new NodeSub($1, $3); }
	| expr '*' expr			  { $$ = new NodeMul($1, $3); }
	| expr '/' expr			  { $$ = new NodeDiv($1, $3); }
	| expr '%' expr			  { $$ = new NodeMod($1, $3); }
	| expr '<' expr			  { $$ = new NodeLt($1, $3); }
	| expr '>' expr			  { $$ = new NodeGt($1, $3); }
	| expr GE expr			  { $$ = new NodeGe($1, $3); }
	| expr LE expr			  { $$ = new NodeLe($1, $3); }
	| expr NE expr			  { $$ = new NodeNe($1, $3); }
	| expr EQ expr			  { $$ = new NodeEq($1, $3); }
	| '(' expr ')'			  { $$ = $2; }
	;

%%
private Node tree; // top level node of the syntax tree
private String filename; // just for error printing
public  Node syntax() { return tree; }

private Yylex lexer; // interface with JFlex lexical analyser
private int yylex() {
	int ret = -1;
	try {
		ret = lexer.yylex();
	} catch (IOException e) {
		System.err.println("Erro: " + e);
	}
	return ret;
}
private int nerrs;
public void yyerror(String msg) {
	nerrs++;
	if (filename == null)
		System.err.println(msg + ": line " + lexer.line() +
				" at or before " + lexer.yytext());
	else
		System.err.println(filename + ": line " + lexer.line() + ": " +
				msg + " at or before " + lexer.yytext());
}
public int errors() { return nerrs; }
public Bparser(Reader r) { nerrs = 0; lexer = new Yylex(r, this); }
public Bparser(Reader r, boolean _debug)
{ this(r); yydebug = _debug; filename = null; }
public Bparser(Reader r, boolean _debug, String infile)
{ this(r, _debug); filename = infile; }
// use this 'main' to test the lexical analyser (with 'yylval' and 'return')
// but without a proper grammar
public static void main(String args[]) throws IOException {
        Bparser parser = new Bparser(new FileReader(args[0]));
	while (parser.yylex() != 0) ;
}
