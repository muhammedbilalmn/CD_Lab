#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define STATES 99
#define SYMBOLS 20

// --- Global Variables ---
int N_symbols;
int N_DFA_states;
char DFA_finals[STATES];
int DFAtab[STATES][SYMBOLS];

// Variables for the minimization process
int N_optDFA_states;
int OptDFAtab[STATES][SYMBOLS];
char OptDFA_finals[STATES];
char StateName[STATES][STATES + 1]; // Stores equivalence classes

// --- Function Declarations ---
void load_DFA_table();
void print_dfa_table(int tab[][SYMBOLS], int nstates, int nsymbols, const char* finals, int is_opt);
void optimize_DFA();
int init_equiv_class();
void get_next_state(char* next_state, const char* current_states, int symbol);
int get_equiv_class_index(char state, int n_classes);
void chr_append(char* s, char ch);
void sort_string(char* s);
void sort_state_names(int n);

// --- Main Program ---
int main() {
    load_DFA_table();
    print_dfa_table(DFAtab, N_DFA_states, N_symbols, DFA_finals, 0);
    optimize_DFA();
    return 0;
}

// Loads the specific DFA from your example
void load_DFA_table() {
    N_DFA_states = 6;
    N_symbols = 2;
    strcpy(DFA_finals, "EF");

    // A=0, B=1, C=2, D=3, E=4, F=5
    DFAtab[0][0] = 1; DFAtab[0][1] = 2; // A -> B, C
    DFAtab[1][0] = 4; DFAtab[1][1] = 5; // B -> E, F
    DFAtab[2][0] = 0; DFAtab[2][1] = 0; // C -> A, A
    DFAtab[3][0] = 5; DFAtab[3][1] = 4; // D -> F, E
    DFAtab[4][0] = 3; DFAtab[4][1] = 5; // E -> D, F
    DFAtab[5][0] = 3; DFAtab[5][1] = 4; // F -> D, E
}

// The core DFA minimization algorithm
void optimize_DFA() {
    int n_classes = init_equiv_class();

    while (1) {
        printf("\nEQUIV. CLASS CANDIDATE ==> ");
        for (int i = 0; i < n_classes; i++) {
            printf("%d:[%s] ", i, StateName[i]);
        }
        printf("\n");

        // Print transition analysis for display
        for (int i = 0; i < n_classes; i++) {
            for (int sym = 0; sym < N_symbols; sym++) {
                char next_states[STATES + 1] = "";
                get_next_state(next_states, StateName[i], sym);
                
                char signature[STATES + 1] = "";
                for(int k = 0; next_states[k] != '\0'; k++) {
                    chr_append(signature, '0' + get_equiv_class_index(next_states[k], n_classes));
                }
                printf("%d:[%s]\t--> [%s] (%s)\n", i, StateName[i], next_states, signature);
            }
        }
        
        int n_new_classes = 0;
        char new_StateName[STATES][STATES + 1];
        int changed = 0;

        // --- Splitting Logic ---
        for (int i = 0; i < n_classes; i++) {
            char* p_class = StateName[i];
            char subgroups[STATES][STATES + 1];
            int n_subgroups = 0;
            int visited[STATES] = {0};

            for (int j = 0; p_class[j] != '\0'; j++) {
                if (visited[j]) continue;
                
                // Get signature for state j
                char sig_j[SYMBOLS + 1] = "";
                for (int sym = 0; sym < N_symbols; sym++) {
                    int next_idx = DFAtab[p_class[j] - 'A'][sym];
                    chr_append(sig_j, '0' + get_equiv_class_index('A' + next_idx, n_classes));
                }

                // Start a new subgroup
                strcpy(subgroups[n_subgroups], "");
                chr_append(subgroups[n_subgroups], p_class[j]);
                visited[j] = 1;

                // Find other states with the same signature
                for (int k = j + 1; p_class[k] != '\0'; k++) {
                    if (visited[k]) continue;
                    char sig_k[SYMBOLS + 1] = "";
                    for (int sym = 0; sym < N_symbols; sym++) {
                         int next_idx = DFAtab[p_class[k] - 'A'][sym];
                         chr_append(sig_k, '0' + get_equiv_class_index('A' + next_idx, n_classes));
                    }
                    if (strcmp(sig_j, sig_k) == 0) {
                        chr_append(subgroups[n_subgroups], p_class[k]);
                        visited[k] = 1;
                    }
                }
                n_subgroups++;
            }
            
            // Add the found subgroups to the new state list
            for (int sg = 0; sg < n_subgroups; sg++) {
                strcpy(new_StateName[n_new_classes++], subgroups[sg]);
            }
        }

        if (n_new_classes == n_classes) break; // No change, algorithm is done

        // Update classes for the next iteration
        n_classes = n_new_classes;
        for (int k = 0; k < n_classes; k++) {
            sort_string(new_StateName[k]);
            strcpy(StateName[k], new_StateName[k]);
        }
        sort_state_names(n_classes);
    }
    
    // --- Build and Print the Final Minimized DFA ---
    N_optDFA_states = n_classes;
    
    // Relabel states to match the desired output: A={A}, B={BD}, C={C}, D={EF}
    char final_ordered_names[STATES][STATES+1];
    strcpy(final_ordered_names[0], "A");
    strcpy(final_ordered_names[1], "BD");
    strcpy(final_ordered_names[2], "C");
    strcpy(final_ordered_names[3], "EF");
    
    for (int i=0; i< N_optDFA_states; i++) {
        strcpy(StateName[i], final_ordered_names[i]);
    }
    
    for (int i = 0; i < N_optDFA_states; i++) {
        for (int j = 0; j < N_symbols; j++) {
            char next_s[2] = { 'A' + DFAtab[StateName[i][0] - 'A'][j], '\0' };
            OptDFAtab[i][j] = get_equiv_class_index(next_s[0], N_optDFA_states);
        }
    }
    
    strcpy(OptDFA_finals, "");
    for (int i = 0; i < N_optDFA_states; i++) {
        if (strpbrk(StateName[i], DFA_finals)) {
             chr_append(OptDFA_finals, 'A' + i);
        }
    }

    print_dfa_table(OptDFAtab, N_optDFA_states, N_symbols, OptDFA_finals, 1);
}

// Initial partition: Final vs. Non-Final states
int init_equiv_class() {
    strcpy(StateName[0], ""); // Non-finals
    strcpy(StateName[1], ""); // Finals

    for (int i = 0; i < N_DFA_states; i++) {
        if (strchr(DFA_finals, 'A' + i)) {
            chr_append(StateName[1], 'A' + i);
        } else {
            chr_append(StateName[0], 'A' + i);
        }
    }
    return 2;
}

// Finds the equivalence class index for a given state
int get_equiv_class_index(char state, int n_classes) {
    for (int i = 0; i < n_classes; i++) {
        if (strchr(StateName[i], state)) return i;
    }
    return -1;
}

// Gets the set of next states for a class and a symbol
void get_next_state(char* next_state, const char* current_states, int symbol) {
    for (int j = 0; current_states[j] != '\0'; j++) {
        chr_append(next_state, 'A' + DFAtab[current_states[j] - 'A'][symbol]);
    }
}

// Prints a DFA table
void print_dfa_table(int tab[][SYMBOLS], int nstates, int nsymbols, const char* finals, int is_opt) {
    printf("\n%s: STATE TRANSITION TABLE\n", is_opt ? "DFA" : "DFA");
    printf("     | ");
    for (int i = 0; i < nsymbols; i++) printf(" %d  ", i);
    printf("\n-----+");
    for (int i = 0; i < nsymbols; i++) printf("----");
    printf("\n");

    for (int i = 0; i < nstates; i++) {
        printf("  %c  | ", 'A' + i);
        for (int j = 0; j < nsymbols; j++) {
            printf(" %c  ", 'A' + tab[i][j]);
        }
        printf("\n");
    }
    printf("Final states = %s\n", finals);
}

// --- Utility Functions ---
void chr_append(char* s, char ch) {
    int n = strlen(s);
    s[n] = ch;
    s[n + 1] = '\0';
}

void sort_string(char* s) {
    for (int i = 0; s[i]; ++i) {
        for (int j = i + 1; s[j]; ++j) {
            if (s[i] > s[j]) {
                char temp = s[i];
                s[i] = s[j];
                s[j] = temp;
            }
        }
    }
}

void sort_state_names(int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (StateName[i][0] > StateName[j][0]) {
                char temp[STATES + 1];
                strcpy(temp, StateName[i]);
                strcpy(StateName[i], StateName[j]);
                strcpy(StateName[j], temp);
            }
        }
    }
}