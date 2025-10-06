%{
#include<stdio.h>
#include<string.h>

// A flag to remember if "bila" was found. 0 = No, 1 = Yes.
int flag = 0;
// A buffer to store the user's input string for the final message.
char input_string[100] = "";
%}

%%

[a-zA-Z]*bila[a-zA-Z]* {
    // This rule now correctly matches first if "bila" is present.
    flag = 1;
    strcpy(input_string, yytext);
}
[a-zA-Z]+ {
    // This rule only matches if the one above did not.
    if (flag == 0) {
        strcpy(input_string, yytext);
    }
}
\n {
    if (flag == 1) {
        printf("%s contains 4 letters of name as substring\n", input_string);
    } else {
        printf("%s does not contain 4 letters of name as substring\n", input_string);
    }
    flag = 0;
    strcpy(input_string, "");
    printf("Enter the string ");
}
[ \t]+          ;
.               ;
%%

int yywrap(void) {
    return 1;
}

int main() {
    printf("Enter the string ");
    yylex();
    return 0;
}