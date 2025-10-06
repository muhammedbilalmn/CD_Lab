#include <stdio.h>
#include <string.h>

#define MAX_INPUT 100
#define MAX_STACK 100

// We need to pass the current position of the input pointer to check()
void check(int input_pos);

char input[MAX_INPUT], stack[MAX_STACK];
int input_len, stack_top = -1;

int main() {
    printf("GRAMMAR is:\nE -> E+E | E*E | (E) | id\n");
    printf("Enter input string: ");
    scanf("%s", input);
    input_len = strlen(input);

    printf("\nStack\t\tInput\t\tAction\n");
    printf("------------------------------------------------------\n");
    printf("$\t\t%s$\t\t\n", input); // Initial state

    for (int i = 0; i < input_len; i++) {
        // --- SHIFT Operation ---
        if (input[i] == 'i' && input[i + 1] == 'd') {
            stack[++stack_top] = 'i';
            stack[++stack_top] = 'd';
            stack[stack_top + 1] = '\0';
            i++; // Skip the 'd' in the next iteration
            printf("$%s\t\t%s$\t\tSHIFT -> id\n", stack, input + i + 1);
            check(i + 1);
        } else {
            stack[++stack_top] = input[i];
            stack[stack_top + 1] = '\0';
            printf("$%s\t\t%s$\t\tSHIFT -> %c\n", stack, input + i + 1, input[i]);
            check(i + 1);
        }
    }

    if (stack_top == 0 && stack[0] == 'E') {
        printf("------------------------------------------------------\n");
        printf("\nInput string is VALID.\n");
    } else {
        printf("------------------------------------------------------\n");
        printf("\nInput string is INVALID.\n");
    }
    return 0;
}

// Corrected check() function
void check(int input_pos) {
    int handle_size;
    char *handle;
    int reduced; // Flag to control the loop

    // Use a do-while loop that continues ONLY if a reduction happened
    do {
        reduced = 0; // Reset flag for this pass
        handle = NULL;

        // Check for handles on top of the stack
        if (stack_top >= 1 && stack[stack_top - 1] == 'i' && stack[stack_top] == 'd') {
            handle = "id";
            handle_size = 2;
        } else if (stack_top >= 2 && stack[stack_top - 2] == 'E' && stack[stack_top - 1] == '+' && stack[stack_top] == 'E') {
            handle = "E+E";
            handle_size = 3;
        } else if (stack_top >= 2 && stack[stack_top - 2] == 'E' && stack[stack_top - 1] == '*' && stack[stack_top] == 'E') {
            handle = "E*E";
            handle_size = 3;
        } else if (stack_top >= 2 && stack[stack_top - 2] == '(' && stack[stack_top - 1] == 'E' && stack[stack_top] == ')') {
            handle = "(E)";
            handle_size = 3;
        }

        // If a handle was found, perform the reduction
        if (handle != NULL) {
            reduced = 1; // A reduction happened, so we might need to loop again
            stack_top -= handle_size - 1;
            stack[stack_top] = 'E';
            stack[stack_top + 1] = '\0';
            
            // Correctly print the remaining input
            printf("$%s\t\t%s$\t\tREDUCE E -> %s\n", stack, input + input_pos, handle);
        }
    } while (reduced); // Loop again only if a reduction was made
}