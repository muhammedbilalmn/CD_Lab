%{
#include "y.tab.h"
#include <stdlib.h> // Required for atoi()
%}

%%
[0-9]+  { yylval= atoi(yytext); return NUMBER; }
"+"     { return '+'; }
"-"     { return '-'; }
"*"     { return '*'; }
"/"     { return '/'; }
"("     { return '('; }
")"     { return ')'; }
\n      { return '\n'; } /* <-- ADDED THIS RULE */
[ \t]+  { /* Ignore spaces and tabs */ }
.       { printf("Unexpected character: %s\n", yytext); }
%%

int yywrap(void) {
    return 1;
}