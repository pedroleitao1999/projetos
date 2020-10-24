%{
#include <stdlib.h>
#include <string.h>
#include "node.h"
#include "y.tab.h"
char *dupstr(const char *s);
extern void yyerror(const char *msg);
int running = 0;
%}

%%
"program" if(running == 0) { running = 1; return PROGRAM; } else { yyerror("Program already initialized"); }
"module" if(running == 0) { running = 1; return MODULE; } else { yyerror("Module already initialized"); }
"end"   if(running == 1) { running = 0; return END; }

[.|\n]*     if(running == 0) { ; /* ignore */ }
("$$".*)|($[^$|\n]+$)  ; /* comment */
"<="  if(running == 1) { return LE; }
">="  if(running == 1) { return GE; }
"~="  if(running == 1) { return NE; }
":="  if(running == 1) { return ATR; }

[ ]+"program" if(running == 1) { return NOME; }
[ ]+"module" if(running == 1) { return NOME; }
[ ]+"end" if(running == 1) { return NOME; }
"start" if(running == 1) { return START; }
"void"  if(running == 1) { return VOID; }
"const" if(running == 1) { return CONST; }
"number"  if(running == 1) { return NUMBER; }
"array" if(running == 1) { return ARRAY; }
"string"  if(running == 1) { return STRING; }
"function"  if(running == 1) { return FUNCTION; }
"public"  if(running == 1) { return PUBLIC; }
"forward" if(running == 1) { return FORWARD; }
"if"  if(running == 1) { return IF; }
"then"  if(running == 1) { return THEN; }
"else"  if(running == 1) { return ELSE; }
"elif"  if(running == 1) { return ELIF; }
"fi"  if(running == 1) { return FI; }
"for" if(running == 1) { return FOR; }
"until" if(running == 1) { return UNTIL; }
"step"  if(running == 1) { return STEP; }
"do"  if(running == 1) { return DO; }
"done"  if(running == 1) { return DONE; }
"repeat"  if(running == 1) { return REPEAT; }
"stop"  if(running == 1) { return STOP; }
"return"  if(running == 1) { return RETURN; }

[A-Za-z][A-Za-z0-9_]*  if(running == 1) { yylval.s = strdup(yytext); return NOME; }
0|([1-9][0-9]*)    if(running == 1) { yylval.i = atoi(yytext); return DEC; }
0[0-7]+    if(running == 1) { yylval.i = atoi(yytext); return OCT; }
0x[0-9A-Fa-f]+   if(running == 1) { yylval.i = atoi(yytext); return HEX; }
0b[01]+    if(running == 1) { yylval.i = atoi(yytext); return BIN; }

\'([^\'\\]|(\\[nrt\'\\])|(\\[0-9A-Fa-f][0-9A-Fa-f]?))\'  if(running == 1) { yylval.s = strdup(yytext); return CHAR; }
\"([^\"]|\\(.|\n))*\"   if(running == 1) { yylval.s = strdup(yytext); return STR; }

[$()[\]&-?^*/%+<>=~|;,!\\#]	if(running == 1) { return *yytext; }

[ \t\n\r]+		if(running == 1) { ; /* ignore whitespace */ }

.   if(running == 1) { yyerror("Unknown character"); }
%%
int yywrap(void) { return 1; }
char *getyytext() { return yytext; }
