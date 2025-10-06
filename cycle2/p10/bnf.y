%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// --- Data Structures ---
struct quad {
    char op[5];
    char arg1[10];
    char arg2[10];
    char result[10];
} QUAD[30];

struct stack {
    int items[100];
    int top;
} stk;

// --- Global Variables ---
int Index = 0;
int tIndex = 0;
int StNo, Ind, tInd;
extern int LineNo;

// --- Function Prototypes ---
void push(int data);
int pop();
void AddQuadruple(const char *op, const char *arg1, const char *arg2, char *result);
void yyerror(const char *s);
int yylex(void);

%}

// --- Yacc Declarations ---
%union {
    char var[10];
}

%token <var> NUM VAR RELOP
%token MAIN IF ELSE WHILE TYPE
%type <var> EXPR ASSIGNMENT CONDITION IFST ELSEST WHILELOOP

%left '-' '+'
%left '*' '/'

%%
/* --- Grammar Rules --- */

PROGRAM: MAIN BLOCK
    ;

BLOCK: '{' CODE '}'
    ;

CODE: BLOCK
    | STATEMENT CODE
    | STATEMENT
    ;

STATEMENT: DESCT ';'
    | ASSIGNMENT ';'
    | CONDST
    | WHILEST
    ;

DESCT: TYPE VARLIST
    ;

VARLIST: VAR ',' VARLIST
    | VAR
    ;

ASSIGNMENT: VAR '=' EXPR {
    strcpy(QUAD[Index].op, "=");
    strcpy(QUAD[Index].arg1, $3);
    strcpy(QUAD[Index].arg2, "");
    strcpy(QUAD[Index].result, $1);
    strcpy($$, QUAD[Index].result);
    Index++;
}
;

EXPR: EXPR '+' EXPR      { AddQuadruple("+", $1, $3, $$); }
    | EXPR '-' EXPR      { AddQuadruple("-", $1, $3, $$); }
    | EXPR '*' EXPR      { AddQuadruple("*", $1, $3, $$); }
    | EXPR '/' EXPR      { AddQuadruple("/", $1, $3, $$); }
    | '-' EXPR           { AddQuadruple("UMIN", $2, "", $$); }
    | '(' EXPR ')'       { strcpy($$, $2); }
    | VAR
    | NUM
    ;

CONDST: IFST {
    Ind = pop();
    sprintf(QUAD[Ind].result, "%d", Index);
    Ind = pop();
    sprintf(QUAD[Ind].result, "%d", Index);
}
    | IFST ELSEST
    ;

IFST: IF '(' CONDITION ')' {
    strcpy(QUAD[Index].op, "==");
    strcpy(QUAD[Index].arg1, $3);
    strcpy(QUAD[Index].arg2, "FALSE");
    strcpy(QUAD[Index].result, "-1");
    push(Index);
    Index++;
} BLOCK {
    strcpy(QUAD[Index].op, "GOTO");
    strcpy(QUAD[Index].arg1, "");
    strcpy(QUAD[Index].arg2, "");
    strcpy(QUAD[Index].result, "-1");
    push(Index);
    Index++;
};

ELSEST: ELSE {
    tInd = pop();
    Ind = pop();
    push(tInd);
    sprintf(QUAD[Ind].result, "%d", Index);
} BLOCK {
    Ind = pop();
    sprintf(QUAD[Ind].result, "%d", Index);
};

CONDITION: VAR RELOP VAR {
    AddQuadruple($2, $1, $3, $$);
    StNo = Index - 1;
}
    | VAR
    | NUM
    ;

WHILEST: WHILELOOP {
    Ind = pop();
    sprintf(QUAD[Ind].result, "%d", StNo);
    Ind = pop();
    sprintf(QUAD[Ind].result, "%d", Index);
}
;

WHILELOOP: WHILE '(' CONDITION ')' {
    strcpy(QUAD[Index].op, "==");
    strcpy(QUAD[Index].arg1, $3);
    strcpy(QUAD[Index].arg2, "FALSE");
    strcpy(QUAD[Index].result, "-1");
    push(Index);
    Index++;
} BLOCK {
    strcpy(QUAD[Index].op, "GOTO");
    strcpy(QUAD[Index].arg1, "");
    strcpy(QUAD[Index].arg2, "");
    strcpy(QUAD[Index].result, "-1");
    push(Index);
    Index++;
}
;

%%
/* --- C Code Section --- */
extern FILE *yyin;

int main(int argc, char *argv[]) {
    FILE *fp;
    int i;
    stk.top = -1; // Initialize stack

    if (argc > 1) {
        fp = fopen(argv[1], "r");
        if (!fp) {
            printf("\nFile not found: %s\n", argv[1]);
            exit(0);
        }
        yyin = fp;
    }

    yyparse();

    // Correctly print the quadruple table header
    printf("\n\n\t\t----------------------------------------\n");
    printf("\t\tPos\tOperator\tArg1\tArg2\tResult\n");
    printf("\t\t----------------------------------------\n");

    for (i = 0; i < Index; i++) {
        printf("\t\t%d\t%s\t\t%s\t%s\t%s\n", i, QUAD[i].op, QUAD[i].arg1, QUAD[i].arg2, QUAD[i].result);
    }
    
    printf("\t\t----------------------------------------\n\n");
    return 0;
}

void push(int data) {
    if (stk.top >= 99) { // Check before incrementing
        printf("\nStack overflow\n");
        exit(0);
    }
    stk.items[++stk.top] = data;
}

int pop() {
    if (stk.top == -1) {
        printf("\nStack underflow\n");
        exit(0);
    }
    return stk.items[stk.top--];
}

void AddQuadruple(const char *op, const char *arg1, const char *arg2, char *result) {
    strcpy(QUAD[Index].op, op);
    strcpy(QUAD[Index].arg1, arg1);
    strcpy(QUAD[Index].arg2, arg2);
    sprintf(QUAD[Index].result, "t%d", tIndex++);
    strcpy(result, QUAD[Index].result);
    Index++;
}

void yyerror(const char *s) {
    printf("\nError on line %d: %s\n", LineNo, s);
}