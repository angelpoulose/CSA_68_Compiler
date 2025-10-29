#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_STATES 100
#define MAX_ALPHABET_SIZE 26
#define MAX_TRANSITIONS 1000 // Max number of transitions to read

// Structure to represent a single transition read from file
typedef struct {
    int current_state;
    char symbol;
    int next_state;
} RawTransition;

// Structure to represent a DFA
typedef struct {
    int num_states;
    int num_alphabet;
    char alphabet[MAX_ALPHABET_SIZE];
    int transitions[MAX_STATES][MAX_ALPHABET_SIZE]; // transitions[current_state][input_symbol_index] = next_state
    bool is_final[MAX_STATES];
    int initial_state;
} DFA;

// Function to get the index of a character in the alphabet
int get_alphabet_index(DFA *dfa, char c) {
    for (int i = 0; i < dfa->num_alphabet; i++) {
        if (dfa->alphabet[i] == c) {
            return i;
        }
    }
    return -1; // Not found
}

// Function to add a character to the alphabet if it's new
void add_to_alphabet(DFA *dfa, char c) {
    if (get_alphabet_index(dfa, c) == -1) { // If symbol not already in alphabet
        if (dfa->num_alphabet < MAX_ALPHABET_SIZE) {
            dfa->alphabet[dfa->num_alphabet++] = c;
            // Sort alphabet for consistent indexing (optional but good practice)
            for (int i = 0; i < dfa->num_alphabet - 1; i++) {
                if (dfa->alphabet[i] > dfa->alphabet[dfa->num_alphabet - 1]) {
                    char temp = dfa->alphabet[i];
                    dfa->alphabet[i] = dfa->alphabet[dfa->num_alphabet - 1];
                    dfa->alphabet[dfa->num_alphabet - 1] = temp;
                }
            }
        } else {
            fprintf(stderr, "Error: Max alphabet size reached.\n");
            exit(EXIT_FAILURE);
        }
    }
}

// Function to print the DFA
void print_dfa(DFA *dfa, const char* title) {
    printf("\n--- %s ---\n", title);
    printf("Number of States: %d\n", dfa->num_states);
    printf("Initial State: q%d\n", dfa->initial_state);
    printf("Final States: ");
    bool first_final = true;
    for (int i = 0; i < dfa->num_states; i++) {
        if (dfa->is_final[i]) {
            if (!first_final) printf(", ");
            printf("q%d", i);
            first_final = false;
        }
    }
    printf("\n");

    printf("Transitions:\n");
    printf("State\\Input\t");
    for (int i = 0; i < dfa->num_alphabet; i++) {
        printf("%c\t", dfa->alphabet[i]);
    }
    printf("\n-----------------------------------\n");
    for (int i = 0; i < dfa->num_states; i++) {
        printf("q%d\t\t", i);
        for (int j = 0; j < dfa->num_alphabet; j++) {
            printf("q%d\t", dfa->transitions[i][j]);
        }
        printf("\n");
    }
    printf("-----------------------------------\n");
}

// Function to minimize the DFA (same as before)
void minimize_dfa(DFA *original_dfa, DFA *minimized_dfa) {
    int n = original_dfa->num_states;
    bool distinguishable[MAX_STATES][MAX_STATES];
    int i, j, k, p, q;
    bool changed;

    // Initialize distinguishable table
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            distinguishable[i][j] = false;
        }
    }

    // Step 1: Mark (p, q) if one is final and the other is not
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            if (original_dfa->is_final[i] != original_dfa->is_final[j]) {
                distinguishable[i][j] = true;
                distinguishable[j][i] = true;
            }
        }
    }

    // Step 2: Iterate to find more distinguishable pairs
    do {
        changed = false;
        for (i = 0; i < n; i++) {
            for (j = i + 1; j < n; j++) { // Iterate only upper triangle
                if (!distinguishable[i][j]) { // If not already marked
                    for (k = 0; k < original_dfa->num_alphabet; k++) {
                        p = original_dfa->transitions[i][k];
                        q = original_dfa->transitions[j][k];

                        if (distinguishable[p][q]) {
                            distinguishable[i][j] = true;
                            distinguishable[j][i] = true;
                            changed = true;
                            break; // Found a distinguishing transition, move to next pair (i,j)
                        }
                    }
                }
            }
        }
    } while (changed);

    // Step 3: Form equivalence classes (find indistinguishable states)
    int equivalence_map[MAX_STATES]; // Maps original state to its new minimized state ID
    int new_state_id = 0;
    bool assigned[MAX_STATES] = {false};

    for (i = 0; i < n; i++) {
        if (!assigned[i]) {
            equivalence_map[i] = new_state_id;
            for (j = i + 1; j < n; j++) {
                if (!distinguishable[i][j]) { // If i and j are indistinguishable
                    equivalence_map[j] = new_state_id;
                    assigned[j] = true;
                }
            }
            assigned[i] = true;
            new_state_id++;
        }
    }
    minimized_dfa->num_states = new_state_id;

    // Step 4: Construct the minimized DFA
    minimized_dfa->num_alphabet = original_dfa->num_alphabet;
    memcpy(minimized_dfa->alphabet, original_dfa->alphabet, sizeof(char) * original_dfa->num_alphabet);

    // Set initial state for minimized DFA
    minimized_dfa->initial_state = equivalence_map[original_dfa->initial_state];

    // Set final states for minimized DFA
    for (i = 0; i < minimized_dfa->num_states; i++) {
        minimized_dfa->is_final[i] = false; // Initialize
    }
    for (i = 0; i < n; i++) {
        if (original_dfa->is_final[i]) {
            minimized_dfa->is_final[equivalence_map[i]] = true;
        }
    }

    // Set transitions for minimized DFA
    for (i = 0; i < n; i++) {
        int current_new_state = equivalence_map[i];
        for (k = 0; k < original_dfa->num_alphabet; k++) {
            int next_original_state = original_dfa->transitions[i][k];
            int next_new_state = equivalence_map[next_original_state];
            minimized_dfa->transitions[current_new_state][k] = next_new_state;
        }
    }
}

int main() {
    DFA original_dfa;
    DFA minimized_dfa;
    const char* filename = "dfa.txt";

    RawTransition raw_transitions[MAX_TRANSITIONS];
    int raw_transition_count = 0;
    int max_state_index = -1;

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    // --- Pass 1: Read raw transitions and determine num_states and alphabet ---
    int s1, s2;
    char sym;
    while (fscanf(file, "%d %c %d", &s1, &sym, &s2) == 3) {
        if (raw_transition_count >= MAX_TRANSITIONS) {
            fprintf(stderr, "Error: Too many transitions in file (max %d).\n", MAX_TRANSITIONS);
            fclose(file);
            return 1;
        }
        raw_transitions[raw_transition_count].current_state = s1;
        raw_transitions[raw_transition_count].symbol = sym;
        raw_transitions[raw_transition_count].next_state = s2;
        raw_transition_count++;

        if (s1 > max_state_index) max_state_index = s1;
        if (s2 > max_state_index) max_state_index = s2;
        add_to_alphabet(&original_dfa, sym); // Populate alphabet dynamically
    }
    fclose(file);

    original_dfa.num_states = max_state_index + 1;
    if (original_dfa.num_states <= 0 || original_dfa.num_states > MAX_STATES) {
        fprintf(stderr, "Error: Inferred number of states (%d) is invalid or exceeds MAX_STATES (%d).\n", original_dfa.num_states, MAX_STATES);
        return 1;
    }

    // Sort the alphabet just in case it wasn't sorted by add_to_alphabet for consistent indexing
    // (This block is optional if add_to_alphabet already sorts, but good for robustness)
    for (int i = 0; i < original_dfa.num_alphabet - 1; i++) {
        for (int j = i + 1; j < original_dfa.num_alphabet; j++) {
            if (original_dfa.alphabet[i] > original_dfa.alphabet[j]) {
                char temp = original_dfa.alphabet[i];
                original_dfa.alphabet[i] = original_dfa.alphabet[j];
                original_dfa.alphabet[j] = temp;
            }
        }
    }

    // --- Pass 2: Populate DFA transitions table ---
    // Initialize all transitions to -1 (or an error state) to detect missing transitions
    for (int i = 0; i < original_dfa.num_states; i++) {
        for (int j = 0; j < original_dfa.num_alphabet; j++) {
            original_dfa.transitions[i][j] = -1; // Indicate an undefined transition
        }
    }

    for (int i = 0; i < raw_transition_count; i++) {
        int current_state = raw_transitions[i].current_state;
        char symbol = raw_transitions[i].symbol;
        int next_state = raw_transitions[i].next_state;

        int symbol_idx = get_alphabet_index(&original_dfa, symbol);
        if (symbol_idx == -1) { // Should not happen if alphabet collected correctly
            fprintf(stderr, "Error: Symbol '%c' not found in alphabet.\n", symbol);
            return 1;
        }
        if (original_dfa.transitions[current_state][symbol_idx] != -1) {
            fprintf(stderr, "Warning: Duplicate transition for state %d on symbol '%c'. Overwriting.\n", current_state, symbol);
        }
        original_dfa.transitions[current_state][symbol_idx] = next_state;
    }

    // Check for undefined transitions (DFA must be complete)
    for (int i = 0; i < original_dfa.num_states; i++) {
        for (int j = 0; j < original_dfa.num_alphabet; j++) {
            if (original_dfa.transitions[i][j] == -1) {
                fprintf(stderr, "Error: Transition for state q%d on symbol '%c' is undefined. DFA must be complete.\n",
                        i, original_dfa.alphabet[j]);
                return 1;
            }
        }
    }

    // --- Prompt user for Initial State ---
    printf("Enter the initial state (0 to %d): ", original_dfa.num_states - 1);
    if (scanf("%d", &original_dfa.initial_state) != 1 ||
        original_dfa.initial_state < 0 || original_dfa.initial_state >= original_dfa.num_states) {
        fprintf(stderr, "Invalid initial state entered.\n");
        return 1;
    }

    // --- Prompt user for Final States ---
    for (int i = 0; i < original_dfa.num_states; i++) {
        original_dfa.is_final[i] = false; // Reset all to non-final
    }

    printf("Enter the final states, space-separated (e.g., 2 3 5). Enter -1 to finish:\n");
    int final_state_input;
    while (scanf("%d", &final_state_input) == 1 && final_state_input != -1) {
        if (final_state_input < 0 || final_state_input >= original_dfa.num_states) {
            fprintf(stderr, "Invalid final state %d. Skipping.\n", final_state_input);
        } else {
            original_dfa.is_final[final_state_input] = true;
        }
    }
    // Clear input buffer after reading integers, in case non-integer was entered to stop loop
    while (getchar() != '\n');

    print_dfa(&original_dfa, "Original DFA (constructed from file and user input)");

    minimize_dfa(&original_dfa, &minimized_dfa);

    print_dfa(&minimized_dfa, "Minimized DFA");

    return 0;
}
