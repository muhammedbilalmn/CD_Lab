#include<stdio.h>
#include<stdlib.h>

// Struct for storing transitions in a linked list
struct node {
    int st;
    struct node *link;
};

// --- Global Variables ---
static int set[20];
int nostate, noalpha, notransition, nofinal, start, finalstate[20], c;
char alphabet[20];
static int e_closure[20][20] = {0};
struct node *transition[20][20] = {NULL};
static int buffer[20]; // Buffer to prevent cycles in findclosure, managed in main

// --- Function Declarations ---
int findalpha(char c_in);
void insert_trantbl(int r, char c_in, int s);
void findclosure(int x, int sta);
void unionclosure(int i);
void findfinalstate();
void print_e_closure(int i);
void print_set(int *s_set);


// Finds the index corresponding to an alphabet character
int findalpha(char c_in) {
    int i;
    for (i = 0; i < noalpha; i++)
        if (alphabet[i] == c_in)
            return i;
    return (999); // Return an indicator for not found
}

// Inserts a new transition into the transition table
void insert_trantbl(int r, char c_in, int s) {
    int j;
    struct node *temp;
    j = findalpha(c_in);
    if (j == 999) {
        printf("Error: Alphabet '%c' not found\n", c_in);
        exit(1);
    }
    temp = (struct node *) malloc(sizeof(struct node));
    if (temp == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    temp->st = s;
    temp->link = transition[r][j];
    transition[r][j] = temp;
}

// Recursively finds the epsilon closure for a given state 'sta'
void findclosure(int x, int sta) {
    struct node *temp;
    
    // If state x has already been visited for this closure calculation, return
    if (buffer[x])
        return;

    e_closure[sta][c++] = x;
    buffer[x] = 1; // Mark as visited

    // Check if the last alphabet is 'e' (epsilon) and there's an epsilon transition
    int epsilon_index = noalpha - 1;
    if (alphabet[epsilon_index] == 'e' && transition[x][epsilon_index] != NULL) {
        temp = transition[x][epsilon_index];
        while (temp != NULL) {
            findclosure(temp->st, sta); // Recur for states reachable by epsilon
            temp = temp->link;
        }
    }
}

// Adds the e-closure of state 'i' to the global 'set' array
void unionclosure(int i) {
    int j = 0, k;
    while (e_closure[i][j] != 0) {
        k = e_closure[i][j];
        set[k] = 1;
        j++;
    }
}

// Identifies and prints the new set of final states without duplicates
void findfinalstate() {
    int i, j, k;
    int new_final_states[20] = {0}; // Array to mark new final states

    // A new state is final if its e-closure contains an original final state
    for (i = 0; i < nofinal; i++) {
        for (j = 1; j <= nostate; j++) {
            for (k = 0; e_closure[j][k] != 0; k++) {
                if (e_closure[j][k] == finalstate[i]) {
                    new_final_states[j] = 1; // Mark state j's e-closure as final
                    break;
                }
            }
        }
    }

    printf("\nFinal states: ");
    int first = 1;
    for (i = 1; i <= nostate; i++) {
        if (new_final_states[i] == 1) {
            if (!first) {
                printf(", ");
            }
            print_e_closure(i);
            first = 0;
        }
    }
    printf("\n");
}

// Helper function to print an e-closure set neatly
void print_e_closure(int i) {
    printf("{");
    if (e_closure[i][0] != 0) {
        printf("q%d", e_closure[i][0]);
        for (int j = 1; e_closure[i][j] != 0; j++) {
            printf(",q%d", e_closure[i][j]);
        }
    }
    printf("}");
}

// Helper function to print a destination state set neatly
void print_set(int *s_set) {
    int first = 1;
    printf("{");
    for (int n = 1; n <= nostate; n++) {
        if (s_set[n] != 0) {
            if (!first) {
                printf(",");
            }
            printf("q%d", n);
            first = 0;
        }
    }
    printf("}");
}

// Use standard int main() instead of void main()
int main() {
    int i, j, k, m, t;
    int r, s;      // Local variables for reading transition source/destination
    char c_in;     // Local char for reading transition alphabet
    struct node *temp;
    int dest_set_size; // Renamed 'newstate' for clarity

    // --- Input Section ---
    printf("Enter the number of alphabets: ");
    scanf("%d", &noalpha);
    getchar(); // Consume the newline character

    printf("NOTE:- [ use letter 'e' as epsilon]\n");
    printf("NOTE:- ['e' must be the last character, if it is present]\n");
    printf("\nEnter alphabets: ");
    for (i = 0; i < noalpha; i++) {
        alphabet[i] = getchar();
        getchar(); // Consume the newline character
    }

    printf("Enter the number of states: ");
    scanf("%d", &nostate);
    printf("Enter the start state: ");
    scanf("%d", &start);
    printf("Enter the number of final states: ");
    scanf("%d", &nofinal);
    printf("Enter the final states: ");
    for (i = 0; i < nofinal; i++)
        scanf("%d", &finalstate[i]);

    printf("Enter no of transition: ");
    scanf("%d", &notransition);
    printf("NOTE:- [Transition is in the form--> state alphabet state]\n");
    printf("NOTE:- [State numbers must be greater than zero]\n");
    printf("\nEnter transitions--\n");
    for (i = 0; i < notransition; i++) {
        // CORRECTED: Use a local char and add a space before %c to handle whitespace
        scanf("%d %c %d", &r, &c_in, &s);
        insert_trantbl(r, c_in, s);
    }

    // --- Epsilon-Closure Calculation ---
    for (i = 1; i <= nostate; i++) {
        c = 0; // Reset index for e_closure array
        for (j = 0; j < 20; j++) { // Clear buffer for the next closure calculation
            buffer[j] = 0;
        }
        findclosure(i, i);
    }
    
    // --- Output NFA without Epsilon ---
    printf("\nEquivalent NFA without epsilon\n");
    printf("-----------------------------------\n");

    // CORRECTED: Print the e-closure of the actual start state, not a hardcoded value
    printf("Start state: ");
    print_e_closure(start);
    printf("\n");

    printf("\nAlphabets: ");
    int num_alpha_to_print = (alphabet[noalpha-1] == 'e') ? noalpha - 1 : noalpha;
    for (i = 0; i < num_alpha_to_print ; i++)
        printf("%c ", alphabet[i]);
    
    printf("\n\nStates: (Each new state is a set of states from the original NFA)\n");
    for (i = 1; i <= nostate; i++) {
        printf("\tNew State (from q%d) = ", i);
        print_e_closure(i);
        printf("\n");
    }

    printf("\nTransitions:\n");
    for (i = 1; i <= nostate; i++) {
        for (j = 0; j < num_alpha_to_print; j++) {
            for (m = 1; m <= nostate; m++)
                set[m] = 0; // Reset the destination set
            
            dest_set_size = 0;

            // For each state 't' in E-CLOSURE(i)
            for (k = 0; e_closure[i][k] != 0; k++) {
                t = e_closure[i][k];
                temp = transition[t][j];
                // Union the e-closures of all states reachable from 't' on the current alphabet
                while (temp != NULL) {
                    unionclosure(temp->st);
                    dest_set_size++;
                    temp = temp->link;
                }
            }

            // CORRECTED: Typo 'newstates' was fixed. We check if a transition exists.
            if (dest_set_size == 0) continue;
            
            printf("\t");
            print_e_closure(i);
            printf(" --%c--> ", alphabet[j]);
            print_set(set);
            printf("\n");
        }
    }

    // CORRECTED: Call the improved function to find and print final states
    findfinalstate();
    
    return 0; // Standard practice for main
}