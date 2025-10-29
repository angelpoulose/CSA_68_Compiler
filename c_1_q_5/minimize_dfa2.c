#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX 20

int n_states, n_symbols;
int dfa[MAX][MAX];
char symbols[MAX];
int final_states[MAX], n_final;
bool reachable[MAX], marked[MAX][MAX];

// Group array maps old states to their representative
int group[MAX];

// --- Function to check if a state is final ---
bool is_final(int state) {
    for (int i = 0; i < n_final; i++) {
        if (final_states[i] == state)
            return true;
    }
    return false;
}

// --- DFS to mark reachable states ---
void dfs(int state) {
    reachable[state] = true;
    for (int i = 0; i < n_symbols; i++) {
        int next = dfa[state][i];
        if (!reachable[next]) {
            dfs(next);
        }
    }
}

// --- Step 1: Eliminate unreachable states ---
void eliminate_unreachable() {
    memset(reachable, 0, sizeof(reachable));
    dfs(0); // assuming Q0 is the start state
}

// --- Step 2: Mark distinguishable pairs ---
void minimize_dfa() {
    eliminate_unreachable();

    // Step 1: Mark pairs (p, q) where one is final, other is not
    memset(marked, 0, sizeof(marked));
    for (int i = 0; i < n_states; i++) {
        for (int j = 0; j < i; j++) {
            if ((is_final(i) && !is_final(j)) || (!is_final(i) && is_final(j))) {
                marked[i][j] = true;
            }
        }
    }

    // Step 2: Iteratively mark other distinguishable pairs
    bool changed;
    do {
        changed = false;
        for (int i = 0; i < n_states; i++) {
            for (int j = 0; j < i; j++) {
                if (marked[i][j]) continue;
                for (int k = 0; k < n_symbols; k++) {
                    int a = dfa[i][k], b = dfa[j][k];
                    if (a == b) continue;
                    int x = a > b ? a : b;
                    int y = a > b ? b : a;
                    if (marked[x][y]) {
                        marked[i][j] = true;
                        changed = true;
                        break;
                    }
                }
            }
        }
    } while (changed);

    // Step 3: Assign groups
    for (int i = 0; i < n_states; i++)
        group[i] = i;

    for (int i = 0; i < n_states; i++) {
        for (int j = 0; j < i; j++) {
            if (!marked[i][j] && reachable[i] && reachable[j]) {
                group[i] = group[j]; // merge i into j's group
                break;
            }
        }
    }
}

// --- Final Print Function in Required Format ---
void print_minimized_dfa(int group[MAX]) {
    printf("\nMinimized DFA Transition Table:\n\n");
    printf("      ");
    for (int i = 0; i < n_symbols; i++) {
        printf("%c     ", symbols[i]);
    }
    printf("\n");

    bool printed[MAX] = {false};
    for (int i = 0; i < n_states; i++) {
        if (!reachable[i] || printed[group[i]])
            continue;

        printed[group[i]] = true;

        printf("Q%d --> ", group[i]);
        for (int j = 0; j < n_symbols; j++) {
            printf("Q%d   ", group[dfa[i][j]]);
        }

        if (is_final(i)) printf("[Final]");
        printf("\n");
    }

    printf("\nTransitions:\n");
    memset(printed, 0, sizeof(printed));
    for (int i = 0; i < n_states; i++) {
        if (!reachable[i] || printed[group[i]])
            continue;
        printed[group[i]] = true;
        for (int j = 0; j < n_symbols; j++) {
            printf("Q%d --%c--> Q%d\n", group[i], symbols[j], group[dfa[i][j]]);
        }
    }

    printf("\nStart State: Q0\n");
    printf("Final State(s): ");
    bool finals[MAX] = {false};
    for (int i = 0; i < n_final; i++) {
        int g = group[final_states[i]];
        if (!finals[g] && reachable[final_states[i]]) {
            printf("Q%d ", g);
            finals[g] = true;
        }
    }
    printf("\n");
}

int main() {
    printf("Enter number of states in DFA: ");
    scanf("%d", &n_states);

    printf("Enter number of input symbols: ");
    scanf("%d", &n_symbols);

    printf("Enter input symbols:\n");
    for (int i = 0; i < n_symbols; i++)
        scanf(" %c", &symbols[i]);

    printf("Enter transition table (state index):\n");
    for (int i = 0; i < n_states; i++) {
        for (int j = 0; j < n_symbols; j++) {
            printf("From Q%d on symbol %c → Q?: ", i, symbols[j]);
            scanf("%d", &dfa[i][j]);
        }
    }

    printf("Enter number of final states: ");
    scanf("%d", &n_final);
    printf("Enter final states: ");
    for (int i = 0; i < n_final; i++) {
        scanf("%d", &final_states[i]);
    }

    minimize_dfa();
    print_minimized_dfa(group);

    return 0;
}

