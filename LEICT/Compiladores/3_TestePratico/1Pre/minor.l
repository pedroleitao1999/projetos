%{
#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "node.h"
#include "y.tab.h"
extern int yyerror(const char *);
static int strsz, intstr(char *s);
static char *str;
%}
%option yylineno
H [A-Fa-f0-9]
%x STRNG CODE FIM
%%
.|\n			;
^"program"		BEGIN CODE; return PROGRAM;
^"module"		BEGIN CODE; return MODULE;
[a-zA-Z]+		;
<CODE>"<="		return LE;
<CODE>">="		return GE;
<CODE>"~="		return NE;
<CODE>":="		return EQ;
<CODE>"$$".*		;
<CODE>\$[^\$]+\$	;
<CODE>"public"		return PUBLIC;
<CODE>"forward"		return FORWARD;
<CODE>"start"		return START;
<CODE>^"end"		BEGIN FIM; return END;
<CODE>"const"		return CONST;
<CODE>"string"		return STRING;
<CODE>"number"		return NUMBER;
<CODE>"array"		return ARRAY;
<CODE>"function"	return FUNCTION;
<CODE>"void"		return VOID;
<CODE>"if"		return IF;
<CODE>"then"		return THEN;
<CODE>"fi"		return FI;
<CODE>"elif"		return ELIF;
<CODE>"else"		return ELSE;
<CODE>"for"		return FOR;
<CODE>"until"		return UNTIL;
<CODE>"step"		return STEP;
<CODE>"do"		return DO;
<CODE>"done"		return DONE;
<CODE>"repeat"		return REPEAT;
<CODE>"stop"		return STOP;
<CODE>"return"		return RETURN;
<CODE>[ \t\n\r]+	;
<CODE>"0x"{H}+		{ yylval.i = intstr(yytext); return INT; }
<CODE>"0b"[01]+		{ yylval.i = intstr(yytext); return INT; }
<CODE>0[0-7]+		{ yylval.i = intstr(yytext); return INT; }
<CODE>[0-9]+		{ yylval.i = intstr(yytext); return INT; }
<CODE>\"([^"\\]|\\.)*\"	{ strsz = yyleng; REJECT; }
<CODE>\"		if (strsz > 0) { BEGIN STRNG; str = malloc(strsz); }
<STRNG>\"			BEGIN CODE; yylval.s = str; return STR;
<STRNG>\\n		strcat(str, "\n");
<STRNG>\\t		strcat(str, "\t");
<STRNG>\\r		strcat(str, "\r");
<STRNG>\\\"		strcat(str, "\"");
<STRNG>\\\\		strcat(str, "\\");
<STRNG>\\{H}{H}?	{ char s[2]; s[1] = 0; s[0] = strtol(yytext+1,0,16);
				strcat(str, s); }
<STRNG>.|\n		strcat(str, yytext);
<CODE>\'\\n\'		yylval.i = '\n'; return CHAR;
<CODE>\'\\t\'		yylval.i = '\t'; return CHAR;
<CODE>\'\\r\'		yylval.i = '\r'; return CHAR;
<CODE>\'\\\'\'		yylval.i = '\''; return CHAR;
<CODE>\'\\\\\'		yylval.i = '\\'; return CHAR;
<CODE>\'\\{H}{H}?\'	yylval.i = strtol(yytext+2,0,16); return CHAR;
<CODE>\'[^\\]\'		yylval.i = yytext[1]; return CHAR;
<CODE>[a-zA-Z][a-zA-Z_0-9]*	{ yylval.s = strdup(yytext); return ID; }
<CODE>.			return yytext[0];
<FIM>.|\n		return 0;
%%
int intstr(char *s)
{
	char *end;
	unsigned long ret;

	if (strlen(s) > 1 && s[1] == 'b')
		ret = strtoul(s+2, &end, 2);
	else
		ret = strtoul(s, &end, 0);
	if (*end != 0) return yyerror("conversion error in literal integer"), -1;
	if (ret > INT_MAX) return yyerror("overflow in literal integer"), -1;
	return ret;
}

int yywrap() { return 1; }
char *getyytext() { return yytext; }
