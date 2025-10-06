%{
#include <stdio.h>
#include <stdlib.h>

void yyerror(const char *s);
int yylex(void);
%}

%token NUMBER

%%
start:
    expression '\n' { printf(" Result is : %d\n", $1); }
    ;

expression:
      expression '+' term   { $$ = $1 + $3; }
    | expression '-' term   { $$ = $1 - $3; }
    | term                  { $$ = $1; }
    ;

term:
      term '*' factor       { $$ = $1 * $3; }
    | term '/' factor       { $$ = $1 / $3; }
    | factor                { $$ = $1; }
    ;

factor:
      '(' expression ')'    { $$ = $2; }
    | NUMBER                { $$ = $1; }
    ;

%%
int main() {
    printf("Enter an arithmetic expression: ");
    yyparse(); // Parse the input
    return 0;
}
void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}