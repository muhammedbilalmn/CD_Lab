%{
#include <stdio.h>

int yylex(void);
void yyerror(const char *s);
int valid = 1;
%}

%token FOR OPERAND OPERATOR COMMA

%%
program:
      /* A program can be empty or have multiple statements */
      /* empty */
    | program statement
    ;

statement:
      for_loop { /* The only valid statement is a for loop */ }
    ;

for_loop:
    FOR '(' partA ';' partB ';' partC ')' '{' program '}'
    ;

/* These rules for the three parts of the for() statement are now corrected */
partA:
      OPERAND OPERATOR OPERAND
    | /* empty */
    ;
partB:
      OPERAND OPERATOR OPERAND
    | /* empty */
    ;
partC:
      OPERAND OPERATOR
    | OPERAND
    | /* empty */
    ;

%%

void yyerror(const char *s) {
    valid = 0;
}

int main(void) {
    printf("Enter string:\n");
    yyparse();
    if (valid) {
        printf("Valid.\n");
    } else {
        printf("Invalid.\n");
    }
    return 0;
}
