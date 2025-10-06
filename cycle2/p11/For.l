%{
#include "y.tab.h"
%}

%%
for                     { return FOR; }
[a-zA-Z0-9_]+           { return OPERAND; }

"=="        { return OPERATOR; }
"!="        { return OPERATOR; }
">="        { return OPERATOR; }
"<="        { return OPERATOR; }
"++"        { return OPERATOR; }
"--"        { return OPERATOR; }
"="         { return OPERATOR; }
"<"         { return OPERATOR; }
">"         { return OPERATOR; }


"("                     { return '('; }
")"                     { return ')'; }
"{"                     { return '{'; }
"}"                     { return '}'; }
";"                     { return ';'; }
","                     { return ','; }

\n                      { /* Ignore newlines and whitespace */ }
[ \t]+                  { /* Ignore newlines and whitespace */ }
.                       ;
%%

int yywrap(void) {
    return 1;
}