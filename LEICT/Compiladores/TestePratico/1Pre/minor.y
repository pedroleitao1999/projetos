%{
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "node.h"
#include "tabid.h"
#include "minor.h"

int yylex(), yyparse(), yyerror(const char*), evaluate(Node*); /* parsers */
static int isId(char*,Node*,int*), isInt(Node*,char*), isAddSub(Node*,char*);
static int isCmp(Node*,char*), isUniInt(Node*,char*), isAddr(Node*);
static int isAssign(Node *n), isCall(char*,Node*,Node*);
static void isFunc(char*,int), isAlloc(Node*,Node*), isRet(Node*), isStr(Node*);
static void isCte(Node*), isFwd(char*,int,Node*), isDim(char*,int,Node*);
static void isPrint(Node*), isCycle();
static char *func;
static int ret, cycle;
%}

%union { int i; char *s; Node *n; }
%token <i> INT CHAR
%token <s> ID STR
%token PROGRAM MODULE END PUBLIC FORWARD STRING NUMBER ARRAY FUNCTION VOID CONST
%token IF THEN FI ELIF ELSE RETURN START FOR UNTIL STEP DO DONE REPEAT STOP

%type<n> lval	decls	gdecls	decl	vardecl	fvar	fvars
%type<i> qualif	const	type	ftype	vdim
%type<n> eqint	eqstr	chars	char	eqvec
%type<n> ints	eqbody	body	ret	loop	instrs
%type<n> instr	elifs	else	expr	exprs	block	main

%token FARGS CHARS INTS ADDR VAR ARGS DECL NIL

%right EQ
%left '|'
%left '&'
%nonassoc '~'
%left '=' NE
%left '<' '>' GE LE
%left '+' '-'
%left '*' '/' '%'
%right '^'
%nonassoc uminus

%%
file	: PROGRAM decls START { func="main"; ret=tPUB + tINT + tFUNC; IDpush(); } main END
	 { IDpop(); evaluate(binNode(PROGRAM, $2, binNode(FUNCTION, binNode(END, TID(func), TINT(ret)), binNode(FARGS, nilNode(NIL), $5)))); }
	| MODULE decls END
	 { evaluate(uniNode(MODULE, $2)); }
	;

decls	:			{ $$ = nilNode(NIL); }
	| gdecls		{ $$ = $1; }
	;

gdecls	: gdecls ';' decl	{ $$ = binNode(DECL, $1, $3); }
	| decl			{ $$ = binNode(DECL, nilNode(NIL), $1); }
	;

decl	: qualif const vardecl { $$ = uniNode(VAR, $3); $$->info = $1+$2+$3->info; isCte($$); }
	| FUNCTION qualif ftype ID { isFunc(func = $4, ret = $2+$3+tFUNC); IDpush(); } fvars { IDchange(ret, $4, $6, 1); } eqbody
		{ $$ = binNode(FUNCTION, binNode(END, TID($4), TINT(ret)), binNode(FARGS, $6, $8)); IDpop(); isFwd(func, ret, $8); }
	| FUNCTION qualif ftype ID { isFunc( func = $4, ret = $2+$3+tFUNC); IDpush(); } eqbody
		{ $$ = binNode(FUNCTION, binNode(END, TID($4), TINT(ret)), binNode(FARGS, nilNode(NIL), $6)); IDpop(); isFwd(func, ret, $6); }
	| error	{ $$ = nilNode(NIL); }
	;

fvar	: NUMBER ID	{ $$ = binNode(NUMBER, TID($2), nilNode(NIL)); $$->info = tINT; }
	| STRING ID	{ $$ = binNode(STRING, TID($2), nilNode(NIL)); $$->info = tSTR; }
	| ARRAY ID vdim	{ $$ = binNode(ARRAY, TID($2), binNode(INTS, TINT($3), nilNode(NIL))); $$->info = tVEC; }
	;

fvars	: fvar			{ $$ = binNode(ARGS, nilNode(NIL), $1);
					IDnew($1->info, $1->SUB(0)->value.s, $1->SUB(1)); }
	| fvars ';' fvar	{ $$ = binNode(ARGS, $1, $3);
					IDnew($3->info, $3->SUB(0)->value.s, $3->SUB(1)); }
	;

vardecl	: NUMBER ID eqint	{ $$ = binNode(NUMBER, TID($2), $3); $$->info = tINT; }
	| STRING ID eqstr	{ $$ = binNode(STRING, TID($2), $3); $$->info = tSTR; }
	| ARRAY ID vdim eqvec	{ $$ = binNode(ARRAY, TID($2), binNode(INTS, TINT($3), $4)); $$->info = tVEC; isDim($2, $3, $4); }
	;

vdim	: '[' INT ']'		{ $$ = $2; isDim(0, $2, 0); }
	|			{ $$ = 0; }
	;

qualif	: PUBLIC		{ $$ = tPUB; }
	| FORWARD		{ $$ = tFWD; }
	|			{ $$ = 0; }
	;

const	: CONST			{ $$ = tCNST; }
	|			{ $$ = 0; }
	;

type	: NUMBER		{ $$ = tINT; }
	| STRING		{ $$ = tSTR; }
	| ARRAY			{ $$ = tVEC; }
	;

ftype	: type			{ $$ = $1; }
	| VOID			{ $$ = tVOID; }
	;

eqint	:			{ $$ = nilNode(NIL); }
	| EQ INT		{ $$ = intNode(INT, $2); }
	| EQ '-' INT		{ $$ = intNode(INT, -$3); }
	;

eqstr	:			{ $$ = nilNode(NIL); }
	| EQ chars		{ isStr($$ = $2); }
	;

chars	: char			{ $$ = binNode(CHARS, nilNode(NIL), $1); $$->info = $1->info; }
	| chars char		{ $$ = binNode(CHARS, $1, $2); $$->info = tSTR; }
	;

char	: INT			{ $$ = TINT($1); $$->info = tINT; }
	| CHAR			{ $$ = intNode(CHAR, $1); $$->info = tINT; }
	| STR			{ $$ = strNode(STR, $1); $$->info = tSTR; }
	;

eqvec	:			{ $$ = nilNode(NIL); }
	| EQ ints		{ $$ = $2; }
	;

ints	: INT			{ $$ = intNode(INT, $1); }
	| '-' INT		{ $$ = intNode(INT, -$2); }
	| ints ',' INT		{ $$ = binNode(INTS, $1, intNode(INT, $3)); }
	| ints ',' '-' INT	{ $$ = binNode(INTS, $1, intNode(INT, -$4)); }
	;

eqbody	: DONE			{ $$ = nilNode(NIL); }
	| DO body		{ $$ = $2; }
	;

main	: fvars ';' instrs
		{ $$ = binNode(START, $1, $3); }
	| instrs
		{ $$ = binNode(START, nilNode(NIL), $1); }
	;

body	: fvars ';' instrs ret
		{ $$ = binNode(START, $1, binNode(';', $3, $4)); }
	| instrs ret
		{ $$ = binNode(START, nilNode(NIL), binNode(';', $1, $2)); }
	;

ret	: RETURN		{ $$ = uniNode(RETURN, nilNode(NIL)); isRet(0); }
	| RETURN expr		{ $$ = uniNode(RETURN, $2); isRet($2); }
	|			{ $$ = nilNode(NIL); }
	;

loop	: ret			{ $$ = $1; }
	| REPEAT		{ $$ = intNode(REPEAT, 1); isCycle(); }
	| STOP			{ $$ = intNode(STOP, 1); isCycle(); }
	;

block	: instrs loop		{ $$ = binNode(';', $1, $2); }
	;

instrs	:			{ $$ = nilNode(NIL); }
	| instrs instr		{ $$ = binNode(';', $1, $2); }
	;

instr	: IF expr THEN block elifs else FI
				{ $$ = binNode(FI, binNode(THEN, binNode(IF, $2, $4), $5), $6); }
	| FOR expr UNTIL expr STEP expr DO { cycle++; } block DONE
				{ $$ = binNode(FOR, binNode(UNTIL, $2, $4), binNode(STEP, $9, $6)); cycle--; }
	| expr '!'		{ $$ = uniNode('!', $1); isPrint($1); }
	| expr ';'		{ $$ = $1; }
	| lval '#' expr ';'	{ $$ = binNode('#', $3, $1); isAlloc($1, $3); }
	;

elifs	:			{ $$ = nilNode(NIL); }
	| elifs ELIF expr THEN block
				{ $$ = binNode(ELIF, $1, binNode(IF, $3, $5)); }
	;

else	:			{ $$ = nilNode(NIL); }
	| ELSE block		{ $$ = $2; }
	;

lval	: ID			{ $$ = TID($1); $$->info = isId($1, 0, 0); }
	| ID '[' expr ']'	{ $$ = binNode('[', $3, TID($1)); $$->info = isId($1, $3, &$$->SUB(1)->info); }
	;

expr	: chars			{ $$ = $1; }
	| lval			{ $$ = $1; }
	| '?'			{ $$ = nilNode('?'); $$->info = tINT; }
	| '&' lval		{ $$ = uniNode(ADDR, $2); $$->info = isAddr($$); }
	| '(' expr ')'		{ $$ = $2; $$->info = $2->info; }
	| expr '+' expr		{ $$ = binNode('+', $1, $3); $$->info = isAddSub($$, "+"); }
	| expr '-' expr		{ $$ = binNode('-', $1, $3); $$->info = isAddSub($$, "-"); }
	| expr '*' expr		{ $$ = binNode('*', $1, $3); $$->info = isInt($$, "*"); }
	| expr '/' expr		{ $$ = binNode('/', $1, $3); $$->info = isInt($$, "/"); }
	| expr '%' expr		{ $$ = binNode('%', $1, $3); $$->info = isInt($$, "%"); }
	| expr '^' expr		{ $$ = binNode('^', $3, $1); $$->info = isInt($$, "^"); }
	| expr '=' expr		{ $$ = binNode('=', $1, $3); $$->info = isCmp($$, "="); }
	| expr NE expr		{ $$ = binNode(NE, $1, $3); $$->info = isCmp($$, "~="); }
	| expr GE expr		{ $$ = binNode(GE, $1, $3); $$->info = isCmp($$, ">="); }
	| expr LE expr		{ $$ = binNode(LE, $1, $3); $$->info = isCmp($$, "<="); }
	| expr '<' expr		{ $$ = binNode('<', $1, $3); $$->info = isCmp($$, "<"); }
	| expr '>' expr		{ $$ = binNode('>', $1, $3); $$->info = isCmp($$, ">"); }
	| expr '&' expr		{ $$ = binNode('&', $1, $3); $$->info = isInt($$, "&"); }
	| expr '|' expr		{ $$ = binNode('|', $1, $3); $$->info = isInt($$, "|"); }
	| '~' expr		{ $$ = uniNode('~', $2); $$->info = isUniInt($$, "~"); }
	| '-' expr %prec uminus	{ $$ = uniNode(uminus, $2); $$->info = isUniInt($$, "-"); }
	| lval EQ expr		{ $$ = binNode(EQ, $3, $1); $$->info = isAssign($$); }
	| ID '(' exprs ')'	{ Node *n; int t = IDfind($1, (void**)&n); $$ = binNode('(', TID($1), $3); $$->user = n; $$->info = $$->SUB(0)->info = t; isCall($1, n, $3); }
	;

exprs	: expr			{ $$ = binNode(',', $1, nilNode(NIL)); }
	| exprs ',' expr	{ $$ = binNode(',', $3, $1); }
	;
%%
char **yynames =
#ifdef YYDEBUG
(char**)yyname;
#else
 0;
#endif

#define tSUB(n,i) tTYPE(n->SUB(i)->info)
static char errstr[200];
static char *types[] = { "void", "number", "string", "array", 0 };

static void isFwd(char *name, int type, Node *n) {
	int attrib = n->attrib;
	if (attrib != NIL && tTYPE(type) == tVEC) attrib = n->SUB(1)->attrib;
	if ((type & tFWD) && attrib != NIL) {
		sprintf(errstr, "forward declarations can not be initialized for '%s'", name);
		yyerror(errstr);
	}
}

static void isCte(Node *n) {
	char *name = n->SUB(0)->SUB(0)->value.s;
	int type = n->info;
	Node *init = n->SUB(0)->SUB(1);
	IDnew(type, name, init);
	n->SUB(0)->SUB(0)->info = type;
	if ((type & tCNST) && (type & tFWD) == 0 && init->attrib == NIL) {
		sprintf(errstr, "constant values must be initialized for '%s'", name);
		yyerror(errstr);
	}
	isFwd(name, type, init);
}

static void isFunc(char *id, int type) {
	void *arg;
	if (IDfind(id, (void**)IDtest) >= 0 && (IDfind(id, &arg) & (tFUNC | tFWD)) == (tFUNC | tFWD))
		/* name exists and is function forward declaration */
		IDreplace(type, id, arg);
	else
		IDnew(type, id, nilNode(NIL));
}

static int isId(char *id, Node *off, int *base) {
	Node *args;
	int type = IDfind(id, (void**)&args);
	if (type < 0) return tVOID;
	if (!off) {
		if ((type & tFUNC)) {
			while (args->attrib != NIL && args->SUB(1)->SUB(1)->attrib != NIL)
				args = args->SUB(0); /* optional argument */
			if (args->attrib != NIL) {
				sprintf(errstr, "function '%s' requires arguments", id);
				yyerror(errstr);
			}
		}
		return type;
	}
	if (tTYPE(type) != tSTR && tTYPE(type) != tVEC) {
		sprintf(errstr, "can not index '%s', not a string or array", id);
		yyerror(errstr);
		return tVOID;
	}
	if (tTYPE(off->info) != tINT) {
		sprintf(errstr, "offset for '%s' indexing must be a number", id);
		yyerror(errstr);
		return tVOID;
	}
	if (base) *base = type;
	return (type & ~(tSTR | tVEC)) | tINT;
}

static int isInt(Node *n, char *op) {
	if (tSUB(n,0) == tINT && tSUB(n,1) == tINT)
		return tINT;
	sprintf(errstr, "invalid arguments for '%s', not numbers", op);
	yyerror(errstr);
	return tVOID;
}

static int isAddSub(Node *n, char *op) {
	if (tSUB(n,0) == tINT && tSUB(n,1) == tINT)
		return tINT;
	if (tSUB(n,0) == tINT && tSUB(n,1) == tVEC)
		return tVEC;
	if (tSUB(n,0) == tVEC && tSUB(n,1) == tINT)
		return tVEC;
	if (!strcmp(op, "-") && tSUB(n,0) == tVEC && tSUB(n,1) == tVEC)
		return tINT;
	sprintf(errstr, "invalid arguments for '%s', incompatible types", op);
	yyerror(errstr);
	return tVOID;
}

static int isCmp(Node *n, char *op) {
	if (tSUB(n,0) == tINT && tSUB(n,1) == tINT)
		return tINT;
	if (tSUB(n,0) == tSTR && tSUB(n,1) == tSTR)
		return tINT;
	if (tSUB(n,0) == tVEC && tSUB(n,1) == tVEC)
		return tINT;
	if (tSUB(n,0) == tINT && n->SUB(0)->attrib == CHARS && n->SUB(0)->SUB(1)->value.i == 0)
		return tINT;
	if (tSUB(n,1) == tINT && n->SUB(1)->attrib == CHARS && n->SUB(1)->SUB(1)->value.i == 0)
		return tINT;
	sprintf(errstr, "invalid arguments for '%s', not of the same type", op);
	yyerror(errstr);
	return tVOID;
}

static int isUniInt(Node *n, char *op) {
	if (tSUB(n,0) == tINT)
		return tINT;
	sprintf(errstr, "invalid argument for '%s', not a number", op);
	yyerror(errstr);
	return tVOID;
}

static int isAddr(Node *n) {
	if (tSUB(n,0) != tINT) {
		sprintf(errstr, "can not take address '&' of non-number");
		yyerror(errstr);
		return tVOID;
	}
	if (n->SUB(0)->attrib == '[') return IDfind(n->SUB(0)->SUB(1)->value.s,0);
	return tVEC;
}

static int isAssign(Node *n) {
	if (n->SUB(1)->info & tCNST) {
		sprintf(errstr, "constant value for ':='");
		yyerror(errstr);
	}
	if (n->SUB(1)->info & tFUNC) {
		sprintf(errstr, "function for ':='");
		yyerror(errstr);
	}
	if (tSUB(n,0) == tINT && n->SUB(0)->attrib == CHARS && n->SUB(0)->SUB(1)->value.i == 0)
		return tSUB(n,0);
	if (tSUB(n,0) == tINT && tSUB(n,1) == tINT)
		return tINT;
	if (tSUB(n,0) == tSTR && tSUB(n,1) == tSTR)
		return tSTR;
	if (tSUB(n,0) == tVEC && tSUB(n,1) == tVEC)
		return tVEC;
	sprintf(errstr, "invalid argument for ':=', espression is %s while variable is %s", types[tSUB(n,0)], types[tSUB(n,1)]);
	yyerror(errstr);
	return tVOID;
}

static int isCall(char *name, Node *fargs, Node *eargs) {
	if (fargs->attrib != NIL && eargs->attrib != NIL) {
		int pos = isCall(name, fargs->SUB(0), eargs->SUB(1)) + 1;
		if (!pos) return -1;
		if (tSUB(eargs,0) == tINT &&
		    eargs->SUB(0)->attrib == CHARS &&
		    eargs->SUB(0)->SUB(1)->value.i == 0)
			return pos;
		if (tSUB(fargs, 1) != tSUB(eargs, 0)) {
			sprintf(errstr, "invalid argument %d for '%s': expected %s, got %s", pos, name, types[tSUB(fargs, 1)], types[tSUB(eargs, 0)]);
			yyerror(errstr);
		}
		return pos;
	} else if (fargs->attrib != NIL) {
		sprintf(errstr, "missing arguments for '%s'", name);
		yyerror(errstr);
		return -1;
	} else if (eargs->attrib != NIL) {
		sprintf(errstr, "too many arguments for '%s'", name);
		yyerror(errstr);
		return -1;
	}
	return 0;
}

static void isDim(char *name, int dim, Node *init) {
	int cnt;
	if (!name) {
		if (!dim) yyerror("array dimension must be positive");
		return;
	}
	if (!dim) return; /* [] not specified */
	for (cnt = init->attrib != NIL; init->attrib == INTS; cnt++)
		init = init->SUB(0);
	if (cnt > dim) {
		sprintf(errstr, "too many initializers '%d' for '%s[%d]'", cnt, name, dim);
		yyerror(errstr);
	}
}

static void isPrint(Node *n) {
	if (tTYPE(n->info) == tVOID) yyerror("can not print void expression");
}

static void isCycle() {
	if (!cycle) yyerror("'repeat' or 'stop' outside 'for' block");
}

static void isStr(Node *n) {
	if (n->info != tSTR) yyerror("invalid string initializer");
}

static void isRet(Node *n) {
	if (!n) {
		if (tTYPE(ret) != tVOID) {
			sprintf(errstr, "invalid expression in return for void function '%s'", func);
			yyerror(errstr);
		}
	} else if (tTYPE(n->info) != tTYPE(ret)) {
		sprintf(errstr, "invalid return type (%s) for '%s' (%s)",
			types[tTYPE(n->info)], func, types[tTYPE(ret)]);
		yyerror(errstr);
	}
}

static void isAlloc(Node *lval, Node *expr) {
	if (lval->info & tCNST) {
		sprintf(errstr, "constant value for '#'\n");
		yyerror(errstr);
	}
	if (tTYPE(lval->info) == tINT || tTYPE(expr->info) != tINT) {
		sprintf(errstr, "invalid arguments for '#', use array or string and number");
		yyerror(errstr);
	}
}
