%{
#include <stdio.h>
#include <string.h>

char pattern[100];  // Stores the 4-character input pattern

// Function to check if pattern exists in the string
int contains_pattern(const char *text) {
    return strstr(text, pattern) != NULL;
}
%}

%%
.*\n     {
            if (contains_pattern(yytext)) {
                printf("Rejected: contains '%s'\n", pattern);
            } else {
                printf("Accepted: %s", yytext);
            }
        }
.|\n     ; // Ignore other characters
%%

int main() {
    printf("Enter first 4 characters of your name: ");
    fflush(stdout);
    fgets(pattern, sizeof(pattern), stdin);
    pattern[strcspn(pattern, "\n")] = '\0';  // Remove newline if present

    printf("Now enter strings (Ctrl+D to stop):\n");
    yylex();
    return 0;
}

