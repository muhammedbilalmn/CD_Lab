#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Structure to hold a single Three-Address Code instruction
struct tac {
    char op[5];
    char arg1[10];
    char arg2[10];
    char result[10];
};

// A simple array to hold our TAC program
// Example Program: a = b * c + d
struct tac code[] = {
    {"*", "b", "c", "t1"},       // t1 = b * c
    {"+", "t1", "d", "a"},      // a = t1 + d
    {"if", "a", "b", "L1"},     // if a < b goto L1
    {"=", "10", "", "c"},        // c = 10
    {"goto", "L2", "", ""},      // goto L2
    {"label", "L1", "", ""},     // L1:
    {"=", "20", "", "c"},        // c = 20
    {"label", "L2", "", ""}      // L2:
};

// Function to check if a string is a number
int is_number(const char* str) {
    if (str == NULL || *str == '\0') {
        return 0;
    }
    while (*str) {
        if (!isdigit(*str)) {
            return 0;
        }
        str++;
    }
    return 1;
}

int main() {
    int num_instructions = sizeof(code) / sizeof(code[0]);
    printf("_start:\n\n"); // Entry point for the assembly program

    // Iterate through each TAC instruction
    for (int i = 0; i < num_instructions; i++) {
        struct tac current = code[i];
        printf("; TAC: %s = %s %s %s\n", current.result, current.arg1, current.op, current.arg2);

        // --- Code Generation for Arithmetic Operations (+, -, *, /) ---
        if (strcmp(current.op, "+") == 0 || strcmp(current.op, "-") == 0 ||
            strcmp(current.op, "*") == 0 || strcmp(current.op, "/") == 0) {
            
            // Load arg1 into AX register
            if (is_number(current.arg1)) {
                 printf("    MOV AX, %s\n", current.arg1);
            } else {
                 printf("    MOV AX, [%s]\n", current.arg1);
            }
           
            // Load arg2 into BX register
            if (is_number(current.arg2)) {
                printf("    MOV BX, %s\n", current.arg2);
            } else {
                printf("    MOV BX, [%s]\n", current.arg2);
            }

            // Perform the operation
            if (strcmp(current.op, "+") == 0) printf("    ADD AX, BX\n");
            if (strcmp(current.op, "-") == 0) printf("    SUB AX, BX\n");
            if (strcmp(current.op, "*") == 0) printf("    MUL BX\n"); // Result is in AX
            if (strcmp(current.op, "/") == 0) {
                printf("    XOR DX, DX\n"); // Clear DX for division
                printf("    DIV BX\n");     // Quotient in AX, Remainder in DX
            }

            // Store the result from AX into the result variable
            printf("    MOV [%s], AX\n\n", current.result);
        }

        // --- Code Generation for Assignment (=) ---
        else if (strcmp(current.op, "=") == 0) {
            // Load arg1 into AX
            if (is_number(current.arg1)) {
                printf("    MOV AX, %s\n", current.arg1);
            } else {
                printf("    MOV AX, [%s]\n", current.arg1);
            }
            // Store AX into the result
            printf("    MOV [%s], AX\n\n", current.result);
        }
        
        // --- Code Generation for Conditional Jump (if) ---
        // This example implements "if arg1 < arg2 goto LBL"
        else if (strcmp(current.op, "if") == 0) {
             // Load arg1 into AX
            if (is_number(current.arg1)) {
                 printf("    MOV AX, %s\n", current.arg1);
            } else {
                 printf("    MOV AX, [%s]\n", current.arg1);
            }
           
            // Load arg2 into BX
            if (is_number(current.arg2)) {
                printf("    MOV BX, %s\n", current.arg2);
            } else {
                printf("    MOV BX, [%s]\n", current.arg2);
            }
            
            printf("    CMP AX, BX\n");
            printf("    JL %s\n\n", current.result); // Jump if Less
        }
        
        // --- Code Generation for Unconditional Jump (goto) ---
        else if (strcmp(current.op, "goto") == 0) {
            printf("    JMP %s\n\n", current.arg1);
        }
        
        // --- Code Generation for Labels ---
        else if (strcmp(current.op, "label") == 0) {
            printf("%s:\n\n", current.arg1);
        }
    }
    
    // Standard program exit sequence for 8086
    printf("\n; Program exit\n");
    printf("    MOV AX, 0x4c00\n");
    printf("    INT 0x21\n");
    
    return 0;
}