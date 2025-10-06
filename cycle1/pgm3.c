#include <stdio.h>
#include <stdlib.h>

// Struct to represent a set of NFA states, which corresponds to one DFA state
struct DfaState {
    int nst[20];
};

// --- Global Variables ---
int nostate, noalpha, notransition, nofinal, start;
char alphabet[20];
int finalstate[20];
struct node {
    int st;
    struct node* link;
} *transition[20][20] = {NULL};

// Array to store the discovered DFA states
struct DfaState dfa_states[20];
int complete = -1; // Index for the last discovered DFA state

// --- Function Declarations ---
int findalpha(char c);
void insert_transition(int r, char c, int s);
void find_e_closure(int state, int closure_set[]);
int compare_states(struct DfaState a, struct DfaState b);
int add_dfa_state(struct DfaState new_state);
void print_dfa_state(struct DfaState state);

// Main function with standard 'int' return type
int main() {
    int i, j, k, r, s;
    char c_in; // Local variable for character input
    
    printf("NOTE:- Use 'e' for epsilon.\n");
    printf("NOTE:- 'e' must be the last alphabet character if present.\n\n");

    printf("Enter the number of alphabets: ");
    scanf("%d", &noalpha);
    getchar();

    printf("Enter the alphabets: ");
    for (i = 0; i < noalpha; i++) {
        alphabet[i] = getchar();
        getchar();
    }

    printf("Enter the number of states: ");
    scanf("%d", &nostate);
    printf("Enter the start state: ");
    scanf("%d", &start);
    printf("Enter the number of final states: ");
    scanf("%d", &nofinal);
    printf("Enter the final states: ");
    for (i = 0; i < nofinal; i++) {
        scanf("%d", &finalstate[i]);
    }

    printf("Enter the number of transitions: ");
    scanf("%d", &notransition);
    printf("NOTE:- Transitions are in the form: state alphabet state\n");
    printf("Enter Transitions:\n");
    for (i = 0; i < notransition; i++) {
        // CORRECTED: Reading into local char variable with space to consume whitespace
        scanf("%d %c %d", &r, &c_in, &s);
        insert_transition(r, c_in, s);
    }

    // --- Subset Construction Algorithm ---
    
    // 1. Initialize the first DFA state with the e-closure of the NFA start state
    struct DfaState current_state = {0}, new_dfa_state = {0};
    int initial_closure[20] = {0};
    find_e_closure(start, initial_closure);

    for(i = 1; i <= nostate; i++) {
        current_state.nst[i] = initial_closure[i];
    }
    add_dfa_state(current_state);

    printf("\n--- Equivalent DFA ---\n");
    printf("Transitions:\n");

    // 2. Process each new DFA state until no more are discovered
    i = 0;
    while (i <= complete) {
        current_state = dfa_states[i];
        
        // Loop through each alphabet symbol (excluding epsilon)
        int num_alpha_for_dfa = (alphabet[noalpha-1] == 'e') ? noalpha - 1 : noalpha;
        for (k = 0; k < num_alpha_for_dfa; k++) {
            
            // a. Find move(current_state, alphabet[k])
            int move_set[20] = {0};
            for (j = 1; j <= nostate; j++) {
                if (current_state.nst[j] != 0) { // If NFA state j is in the current DFA state
                    struct node* temp = transition[j][k];
                    while (temp) {
                        move_set[temp->st] = temp->st;
                        temp = temp->link;
                    }
                }
            }
            
            // b. Find e-closure of the resulting move_set
            int closure_of_move[20] = {0};
            for(int m = 1; m <= nostate; m++) {
                if (move_set[m] != 0) {
                    find_e_closure(m, closure_of_move);
                }
            }
            
            // Check if the resulting state is not an empty set (dead state)
            int is_empty = 1;
            for(int m = 1; m <= nostate; m++) {
                new_dfa_state.nst[m] = closure_of_move[m];
                if (new_dfa_state.nst[m] != 0) is_empty = 0;
            }

            // Print the transition and add the new state if it's not empty
            if (!is_empty) {
                print_dfa_state(current_state);
                printf("--%c--> ", alphabet[k]);
                print_dfa_state(new_dfa_state);
                printf("\n");
                add_dfa_state(new_dfa_state);
            }
        }
        i++;
    }

    // --- Print Final DFA Details ---
    printf("\nStates of DFA are:\n");
    for (i = 0; i <= complete; i++) {
        print_dfa_state(dfa_states[i]);
        printf("\n");
    }

    printf("\nStart State is: ");
    print_dfa_state(dfa_states[0]);
    
    printf("\n\nFinal States are: ");
    for (i = 0; i <= complete; i++) {
        for (j = 1; j <= nostate; j++) {
            for (k = 0; k < nofinal; k++) {
                if (dfa_states[i].nst[j] == finalstate[k]) {
                    print_dfa_state(dfa_states[i]);
                    goto next_state; // Use goto to avoid duplicate printing for the same state
                }
            }
        }
        next_state:;
    }
    printf("\n");

    return 0;
}

// Finds the index of an alphabet character
int findalpha(char c) {
    for (int i = 0; i < noalpha; i++)
        if (alphabet[i] == c) return i;
    return 999;
}

// Inserts a transition into the NFA's transition table
void insert_transition(int r, char c, int s) {
    int j = findalpha(c);
    if (j == 999) {
        printf("Error: alphabet not found.\n");
        exit(1);
    }
    struct node* temp = (struct node*)malloc(sizeof(struct node));
    temp->st = s;
    temp->link = transition[r][j];
    transition[r][j] = temp;
}

// Recursively finds the e-closure of a single state and adds it to a set
void find_e_closure(int state, int closure_set[]) {
    if (closure_set[state]) return; // If already in the set, stop to prevent cycles

    closure_set[state] = state;
    int e_index = noalpha - 1;
    if (alphabet[e_index] == 'e') {
        struct node* temp = transition[state][e_index];
        while (temp) {
            find_e_closure(temp->st, closure_set);
            temp = temp->link;
        }
    }
}

// Compares two DFA states (sets of NFA states) to see if they are identical
int compare_states(struct DfaState a, struct DfaState b) {
    for (int i = 1; i <= nostate; i++)
        if (a.nst[i] != b.nst[i]) return 0; // Not equal
    return 1; // Equal
}

// Adds a new state to the list of DFA states if it's not already present
int add_dfa_state(struct DfaState new_state) {
    for (int i = 0; i <= complete; i++)
        if (compare_states(dfa_states[i], new_state)) return 0; // Already exists
    
    complete++;
    dfa_states[complete] = new_state;
    return 1; // Was newly added
}

// Helper function to print a DFA state set neatly
void print_dfa_state(struct DfaState state) {
    printf("{");
    int first = 1;
    for (int j = 1; j <= nostate; j++) {
        if (state.nst[j] != 0) {
            if (!first) printf(",");
            printf("q%d", state.nst[j]);
            first = 0;
        }
    }
    printf("}");
}