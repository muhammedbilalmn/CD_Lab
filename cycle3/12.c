#include <stdio.h>
#include <string.h>

int main() {
    // Corrected the 'opt' array to be 2D, which is what's needed.
    char stack[20], ip[20], opt[10][10], ter[10];
    int i, j, k, n, top = 0, col, row;

    // Correctly initialize char arrays with the null character '\0'.
    for (i = 0; i < 10; i++) {
        stack[i] = '\0';
        ip[i] = '\0';
        for (j = 0; j < 10; j++) {
            opt[i][j] = '\0';
        }
    }

    printf("Enter the no.of terminals:\n");
    scanf("%d", &n);
    printf("\nEnter the terminals:\n");
    scanf("%s", ter);

    printf("\nEnter the table values:\n");
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            printf("Enter the value for %c %c:", ter[i], ter[j]);
            // Corrected scanf to read a single character and prevent buffer overflow.
            // The space before %c is important to consume leftover newlines.
            scanf(" %c", &opt[i][j]);
        }
    }

    printf("\n**** OPERATOR PRECEDENCE TABLE ****\n");
    for (i = 0; i < n; i++) {
        printf("\t%c", ter[i]);
    }
    printf("\n");

    for (i = 0; i < n; i++) {
        printf("\n%c", ter[i]);
        for (j = 0; j < n; j++) {
            // Corrected array access.
            printf("\t%c", opt[i][j]);
        }
    }

    stack[top] = '$';
    printf("\nEnter the input string: ");
    scanf("%s", ip);
    // Append '$' to the end of the input string to mark the end.
    strcat(ip, "$");

    i = 0;
    printf("\nSTACK\t\t\tINPUT STRING\t\t\tACTION\n");
    printf("\n%s\t\t\t%s\t\t\t", stack, ip);

    // The main parsing loop.
    while (i <= strlen(ip)) {
        for (k = 0; k < n; k++) {
            if (stack[top] == ter[k])
                col = k;
            if (ip[i] == ter[k])
                row = k;
        }

        if ((stack[top] == '$') && (ip[i] == '$')) {
            printf("String is accepted\n");
            break;
        } else if ((opt[col][row] == '<') || (opt[col][row] == '=')) {
            // CORRECTED SHIFT: Only push the input symbol, not the relation.
            stack[++top] = ip[i];
            printf("Shift %c", ip[i]);
            i++;
        } else {
            if (opt[col][row] == '>') {
                // CORRECTED REDUCE: A simple reduce pops the handle from the stack.
                // We assume the handle is the top-most element.
                top--;
                printf("Reduce");
            } else {
                printf("\nString is not accepted");
                break;
            }
        }
        printf("\n");

        // Print current stack
        for (k = 0; k <= top; k++) {
            printf("%c", stack[k]);
        }
        printf("\t\t\t");

        // Print remaining input string
        for (k = i; k < strlen(ip); k++) {
            printf("%c", ip[k]);
        }
        printf("\t\t\t");
    }

    getchar(); // To hold the output screen
}