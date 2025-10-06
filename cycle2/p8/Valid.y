%{
#include <stdio.h>

int valid = 1;
int yylex(void);
void yyerror(const char *s);   // ✅ correct prototype
%}

%token digit letter

%%
start : letter s 
;
s : letter s
  | digit s
  |
;
%%

void yyerror(const char *s){ 
    printf("\nIts not an identifier!\n"); 
    valid = 0; 
}

int main(){
    printf("\nEnter a name to tested for identifier\n");
    yyparse(); 
    if(valid){ 
        printf("\nIt is an identifier!\n");
    }
    return 0;
}
