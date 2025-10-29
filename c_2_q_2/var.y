
%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void yyerror(const char *s);
int yylex(void);
%}

%union {
    char *str;
}

%token <str> IDENTIFIER

%%

input:
    IDENTIFIER    { printf("Valid variable name: %s\n", $1); free($1); }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}

int main() {
    printf("Enter a variable name:\n");
    return yyparse();
}

