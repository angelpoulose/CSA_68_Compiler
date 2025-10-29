%{
#include <stdio.h>
#include <stdlib.h>
void yyerror(const char *s);
int yylex();
%}

%token FOR ID NUM RELOP INCR DECR

%%

stmt:
    FOR '(' assign_stmt ';' condition ';' assign_stmt ')' block
    {
        printf("Valid for loop syntax\n");
    }
;

assign_stmt:
      ID '=' NUM     // i = 0
    | ID INCR        // i++
    | ID DECR        // i--
;

condition:
    ID RELOP NUM
;

block:
    '{' '}'
;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Syntax error: %s\n", s);
}

int main() {
    printf("Enter a for loop:\n");
    yyparse();
    return 0;
}

