#include <stdio.h>
#include <string.h>

char input[50], stack[50];
int top = -1, ip = 0;

void push(char c) {
    stack[++top] = c;
    stack[top+1] = '\0';
}

void popn(int n) {
    top -= n;
    stack[top+1] = '\0';
}

void printAction(const char *action) {
    char inputWithDollar[50];
    snprintf(inputWithDollar, sizeof(inputWithDollar), "%s$", &input[ip]);
    printf("\n%-15s %-15s %-15s", stack, inputWithDollar, action);
}


void check() {
    // Reduction: id → E
    for (int k = 0; k <= top; k++) {
        if (stack[k] == 'i' && stack[k+1] == 'd') {
            stack[k] = 'E';
            // remove 'd'
            for (int t = k+1; t <= top; t++)
                stack[t] = stack[t+1];
            top--;
            printAction("REDUCE TO E");
        }
    }

    // Reduction: E+E → E
    for (int k = 0; k <= top-2; k++) {
        if (stack[k] == 'E' && stack[k+1] == '+' && stack[k+2] == 'E') {
            stack[k] = 'E';
            popn(2); // remove +E
            printAction("REDUCE TO E");
        }
    }

    // Reduction: E*E → E
    for (int k = 0; k <= top-2; k++) {
        if (stack[k] == 'E' && stack[k+1] == '*' && stack[k+2] == 'E') {
            stack[k] = 'E';
            popn(2); // remove *E
            printAction("REDUCE TO E");
        }
    }

    // Reduction: (E) → E
    for (int k = 0; k <= top-2; k++) {
        if (stack[k] == '(' && stack[k+1] == 'E' && stack[k+2] == ')') {
            stack[k] = 'E';
            popn(2); // remove E)
            printAction("REDUCE TO E");
        }
    }
}
int main() {
    printf("GRAMMAR is:\n");
    printf("E -> E+E\nE -> E*E\nE -> (E)\nE -> id\n");

    printf("\nEnter input string: ");
    scanf("%s", input);

    printf("\n%-15s %-15s %-15s", "Stack", "Input", "Action");

    while (input[ip] != '\0') {
        if (input[ip] == 'i' && input[ip+1] == 'd') {
            push('i');
            push('d');
            ip += 2;
            printAction("SHIFT -> id");
            check();
        } else {
            push(input[ip]);
            ip++;
            printAction("SHIFT -> symbol");
            check();
        }
    }

    if (strcmp(stack, "E") == 0)
        printf("\n\nString Accepted.\n");
    else
        printf("\n\nString Rejected.\n");

    return 0;
}

