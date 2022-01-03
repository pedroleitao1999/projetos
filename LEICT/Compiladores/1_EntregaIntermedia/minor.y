%{
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "tabid.h"
#include "node.h"
#define YYDEBUG 1
#define NODEDBUG 1

int yylex();
void yyerror(char *s);
int lbl;
%}

%union { int i; char *s; Node *n; }
%token <i> DEC OCT HEX BIN
%token <s> NOME STR CHAR

%token PROGRAM MODULE START END VOID NUMBER ARRAY STRING FUNCTION IF
%token FOR REPEAT STOP RETURN CONST PUBLIC FORWARD THEN ELSE
%token ELIF FI UNTIL STEP DO DONE
%nonassoc '#' ',' ';' '!'
%right ATR
%left '|'
%left '&'
%nonassoc '~'
%left NE '='
%left '<' '>' LE GE
%left '+' '-'
%left '*' '/' '%'
%right '^'
%nonassoc LOCALIZATION UMINUS '?'
%nonassoc '(' ')' '[' ']'

%type <n> programa modulo declaracao declaracoes funcao corpo variavel
%type <n> variaveis tipo qualificador literal literais
%type <n> instrucao instrucaoFim instrucoes expressao expressoes
%token NIL DECLARATIONS DECLARATION VARIABLES VARIABLE BODY
%token LITERALS LITERAL INSTRUCTIONS INSTRUCTION EXPRESSIONS EXPRESSION

%%

ficheiro : programa	{ printNode($1, 0, (char**)yyname); }
    	| modulo	{ printNode($1, 0, (char**)yyname); }
	;

programa : PROGRAM declaracoes START corpo END	{ $$ = binNode(PROGRAM, $2, uniNode(START, $4) ); }
	;

modulo : MODULE declaracoes END	{ $$ = uniNode(MODULE, $2); }
	;

declaracao : funcao	{ $$ = uniNode(FUNCTION, $1); }
	| variavel	{ $$ = uniNode(VARIABLE, $1); }
	| variavel ATR literais	{ $$ = binNode(VARIABLE, $1, uniNode(LITERALS, $3)); }
	| CONST variavel	{ $$ = uniNode(VARIABLE, $2); }
	| CONST variavel ATR literais	{ $$ = binNode(VARIABLE, $2, uniNode(LITERALS, $4) ); }
	| qualificador variavel	{ $$ = binNode(VARIABLE, $1, $2); }
	| qualificador variavel ATR literais	{ $$ = binNode(VARIABLE, binNode(VARIABLE, $1, $2), uniNode(LITERALS, $4)); }
	| qualificador CONST variavel	{ $$ = binNode(VARIABLE, $1, $3); }
	| qualificador CONST variavel ATR literais { $$ = binNode(VARIABLE, binNode(VARIABLE, $1, $3), uniNode(LITERALS, $5)); }
	;

declaracoes :
	| declaracao 			{ $$ = $1; }
	| declaracoes ';' declaracao	{ $$ = binNode(DECLARATIONS, $1, $3); }
	;

funcao : FUNCTION tipo NOME DONE	{ $$ = strNode(NOME, $3); }
	| FUNCTION tipo NOME DO corpo	{ $$ = binNode(FUNCTION, strNode(NOME, $3), $5 ); }
	| FUNCTION tipo NOME variaveis DONE	{ $$ = binNode(FUNCTION, strNode(NOME, $3), $4 ); }
	| FUNCTION tipo NOME variaveis DO corpo	{ $$ = triNode(FUNCTION, strNode(NOME, $3), $4, $6 ); }
	| FUNCTION VOID NOME DONE	{ $$ = strNode(NOME, $3); }
   	| FUNCTION VOID NOME DO corpo	{ $$ = binNode( FUNCTION, strNode(NOME, $3), $5); }
    	| FUNCTION VOID NOME variaveis DONE	{ $$ = binNode( FUNCTION, strNode(NOME, $3), $4); }
   	| FUNCTION VOID NOME variaveis DO corpo	{ $$ = triNode( FUNCTION, strNode(NOME, $3), $4, $6); }
    	| FUNCTION qualificador tipo NOME DONE	{ $$ = strNode(NOME, $4); }
    	| FUNCTION qualificador tipo NOME DO corpo	{ $$ = binNode( FUNCTION, strNode(NOME, $4), $6); }
    	| FUNCTION qualificador tipo NOME variaveis DONE	{ $$ = binNode( FUNCTION, strNode(NOME, $4), $5); }
    	| FUNCTION qualificador tipo NOME variaveis DO corpo	{ $$ = triNode( FUNCTION, strNode(NOME, $4), $5, $7); }
    	| FUNCTION qualificador VOID NOME DONE		{ $$ = strNode(NOME, $4); }
    	| FUNCTION qualificador VOID NOME DO corpo	{ $$ = binNode( FUNCTION, strNode(NOME, $4), $6); }
    	| FUNCTION qualificador VOID NOME variaveis DONE	{ $$ = binNode( FUNCTION, strNode(NOME, $4), $5); }
    	| FUNCTION qualificador VOID NOME variaveis DO corpo	{ $$ = triNode( FUNCTION, strNode(NOME, $4), $5, $7); }
	;

corpo :
	| variaveis		{ $$ = uniNode(BODY, $1); }
	| instrucoes		{ $$ = uniNode(BODY, $1); }
    	| variaveis instrucoes	{ $$ = binNode(BODY, $1, $2); }
    	;

variavel : tipo NOME '[' DEC ']'	{ $$ = strNode(NOME, $2); }
    	| tipo NOME			{ $$ = strNode(NOME, $2); }
	;

variaveis : variavel			{ $$ =  $1; }
    	| variavel ';'			{ $$ = $1; }
    	| variaveis variavel		{ $$ = binNode(VARIABLES, $1, uniNode(VARIABLE, $2) ); }
	| variaveis variavel ';'	{ $$ = binNode(VARIABLES, $1, uniNode(VARIABLE, $2) ); }
	;

tipo : NUMBER
	| STRING
	| ARRAY
	;

qualificador : PUBLIC
	| FORWARD
	;

literal : DEC	{ $$ = intNode(DEC, $1); }
    	| OCT	{ $$ = intNode(OCT, $1); }
    	| HEX	{ $$ = intNode(HEX, $1); }
    	| BIN	{ $$ = intNode(BIN, $1); }
	| CHAR	{ $$ = strNode(CHAR, $1); }
	| STR	{ $$ = strNode(STR, $1); }
	;

literais : literal		{ $$ = $1; }
	| literais literal	{ $$ = binNode(LITERALS, $1, $2); }
	| literais ',' literal	{ $$ = binNode(LITERALS, $1, $3); }
	;

instrucao : IF expressao THEN instrucoes expressoes FI	{ $$ = binNode(IF, $2, binNode(THEN, $4, $5) ); }
    	| IF expressao THEN instrucoes expressoes ELSE instrucoes FI	{ $$ = triNode(IF, $2, binNode(THEN, $4, $5), uniNode(ELSE, $7) ); }
    	| IF expressao THEN instrucoes FI 	{ $$ = binNode(IF, $2, uniNode(THEN, $4) ); }
    	| IF expressao THEN instrucoes ELSE instrucoes FI	{ $$ = triNode(IF, $2, uniNode(THEN, $4), uniNode(ELSE, $6) ); }
 	| FOR expressao UNTIL expressao STEP expressao DO instrucoes DONE	{ $$ = triNode(IF, $2, binNode(UNTIL, $4, $6), uniNode(DO, $8) ); }
	| expressao ';'			{ $$ = uniNode(EXPRESSION, $1); }
	| expressao '!'			{ $$ = uniNode(EXPRESSION, $1); }
	| NOME '#' expressao ';'	{ $$ = binNode(EXPRESSION, strNode(NOME, $1), $3); }
	| instrucaoFim			{ $$ = uniNode(EXPRESSION, $1); }
	;

instrucaoFim : RETURN
    	| RETURN expressao	{ $$ = uniNode(RETURN, $2); }
    	| STOP
    	| REPEAT
    	;

instrucoes : instrucao		{ $$ = $1; }
    	| instrucoes instrucao	{ $$ = binNode(INSTRUCTIONS, $1, $2); }
    	;

expressao : NOME			{ $$ = strNode(NOME, $1); }
    	| NOME '[' expressao ']'	{ $$ = binNode(ARRAY, strNode(NOME, $1), $3); }
    	| NOME '(' expressao ')' '[' expressao ']'	{ $$ = triNode(ARRAY, strNode(NOME, $1), $3, $6); }
    	| literais	{ $$ = uniNode(LITERALS, $1); }
    	| '(' expressao ')'	{ $$ = $2; }
    	| NOME '(' expressao ')'	{ $$ = binNode(FUNCTION, strNode(NOME, $1), $3); }
    	| expressao ',' expressao	{ $$ = binNode(EXPRESSIONS, $1, $3); }
    	| '-' expressao %prec UMINUS	{ $$ = uniNode(UMINUS, $2); }
    	| '&' NOME %prec LOCALIZATION	{ $$ = uniNode(LOCALIZATION, strNode(NOME, $2)); }
    	| '&' NOME '(' expressao ')' %prec LOCALIZATION	{ $$ = binNode(LOCALIZATION, strNode(NOME, $2), $4); }
	| expressao '^' expressao	{ $$ = binNode('^', $1, $3); }
	| expressao '*' expressao	{ $$ = binNode('*', $1, $3); }
	| expressao '/' expressao	{ $$ = binNode('/', $1, $3); }
	| expressao '%' expressao	{ $$ = binNode('%', $1, $3); }
	| expressao '+' expressao	{ $$ = binNode('+', $1, $3); }
	| expressao '-' expressao	{ $$ = binNode('-', $1, $3); }
	| expressao '<' expressao	{ $$ = binNode('<', $1, $3); }
	| expressao '>' expressao	{ $$ = binNode('>', $1, $3); }
	| expressao LE expressao	{ $$ = binNode(LE, $1, $3); }
	| expressao GE expressao	{ $$ = binNode(GE, $1, $3); }
	| expressao NE expressao	{ $$ = binNode(NE, $1, $3); }
	| expressao '=' expressao	{ $$ = binNode('=', $1, $3); }
	| '~' expressao			{ $$ = uniNode('~', $2); }
	| expressao '&' expressao	{ $$ = binNode('&', $1, $3); }
	| expressao '|' expressao	{ $$ = binNode('|', $1, $3); }
	| expressao ATR expressao	{ $$ = binNode(ATR, $1, $3); }
	| '?'
	;

expressoes : ELIF expressao THEN instrucoes	{ $$ = binNode(ELIF, $2, uniNode(THEN, $4) ); }
    	| expressoes ELIF expressao THEN instrucoes	{ $$ = triNode(EXPRESSIONS, $1, uniNode(ELIF, $3), uniNode(THEN, $5) ); }
    	;

%%

char *mklbl(int n) {
  static char buf[20];
  sprintf(buf, "_i%d", n);
  return strdup(buf);
}

char **yynames =
#if YYDEBUG > 0
		 (char**)yyname;
#else
		 0;
#endif