#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

char ip_sym[15], op[50], tmp[50];
int ip_ptr = 0;

void e_prime(), e(), t_prime(), t(), f(), advance(), remove_epsilons();

void remove_epsilons() {
    int n = 0;
    for (int i = 0; op[i] != '\0'; i++) {
        if (op[i] != 'e') {
            tmp[n++] = op[i];
        }
    }
    tmp[n] = '\0';
    strcpy(op, tmp);
}

void e() {
    strcpy(op, "TE'");
    printf("E=%-25s E -> TE'\n", op);
    t();
    e_prime();
}

void e_prime() {
    remove_epsilons();
    int n, i, l = strlen(op);
    for (n = 0; n < l - 1 && !(op[n] == 'E' && op[n + 1] == '\''); n++);

    if (ip_sym[ip_ptr] == '+') {
        for (i = l; i >= n + 2; i--) op[i + 2] = op[i];
        op[n++] = '+'; op[n++] = 'T'; op[n++] = 'E'; op[n++] = '\'';
        printf("E=%-25s E' -> +TE'\n", op);
        advance();
        t();
        e_prime();
    } else {
        // *** MODIFIED LOGIC HERE ***
        // Replace E' (2 chars) with e (1 char) and shift the string left
        op[n] = 'e';
        for (i = n + 1; i < l; i++) {
            op[i] = op[i + 1]; // Shift characters left
        }
        printf("E=%-25s E' -> e\n", op);
    }
}

void t() {
    remove_epsilons();
    int n, i, l = strlen(op);
    for (n = 0; n < l && !(op[n] == 'T' && op[n + 1] != '\''); n++);
    
    for (i = l; i >= n + 1; i--) op[i + 2] = op[i];
    op[n++] = 'F'; op[n++] = 'T'; op[n++] = '\'';
    printf("E=%-25s T -> FT'\n", op);
    f();
    t_prime();
}

void t_prime() {
    remove_epsilons();
    int n, i, l = strlen(op);
    for (n = 0; n < l - 1 && !(op[n] == 'T' && op[n + 1] == '\''); n++);

    if (ip_sym[ip_ptr] == '*') {
        for (i = l; i >= n + 2; i--) op[i + 2] = op[i];
        op[n++] = '*'; op[n++] = 'F'; op[n++] = 'T'; op[n++] = '\'';
        printf("E=%-25s T' -> *FT'\n", op);
        advance();
        f();
        t_prime();
    } else {
        // *** MODIFIED LOGIC HERE ***
        // Replace T' (2 chars) with e (1 char) and shift the string left
        op[n] = 'e';
        for (i = n + 1; i < l; i++) {
            op[i] = op[i + 1]; // Shift characters left
        }
        printf("E=%-25s T' -> e\n", op);
    }
}

void f() {
    remove_epsilons();
    int n;
    for (n = 0; op[n] != 'F'; n++);

    if (tolower(ip_sym[ip_ptr]) == 'i') {
        op[n] = 'i';
        printf("E=%-25s F -> i\n", op);
        advance();
    } else {
        printf("\nSyntax error: This version does not handle parentheses.\n");
        exit(1);
    }
}

void advance() {
    ip_ptr++;
}

int main() {
    printf("Grammar without left recursion\n");
    printf("\tE->TE'\n\tE'->+TE'|e\n\tT->FT'\n\tT'->*FT'|e\n\tF->(E)|i\n\n");
    printf("Enter the input expression:");
    scanf("%s", ip_sym);

    printf("\n%-25s %s\n", "Expressions", "Sequence of production rules");
    
    e();

    if (ip_sym[ip_ptr] == '\0') {
        remove_epsilons();
        printf("\nE=%s\n", op);
    } else {
        printf("\nERROR: The string could not be parsed.\n");
    }

    return 0;
}