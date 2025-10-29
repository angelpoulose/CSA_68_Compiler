%{
#include <stdio.h>
#include <stdlib.h>
int yylex();
int yyerror(const char*);
%}

%token NUMBER
%token PLUS MINUS MUL DIV
%token LPAREN RPAREN

%left PLUS MINUS
%left MUL DIV
%right UMINUS

%%
program:
      program expr '\n'   { printf("= %d\n", $2); }
    |                     /* empty */
    ;

expr:
      expr PLUS expr       { $$ = $1 + $3; }
    | expr MINUS expr      { $$ = $1 - $3; }
    | expr MUL expr        { $$ = $1 * $3; }
    | expr DIV expr        { 
                              if ($3 == 0) {
                                  printf("Error: Division by zero\n");
                                  exit(1);
                              }
                              $$ = $1 / $3;
                            }
    | LPAREN expr RPAREN   { $$ = $2; }
    | MINUS expr %prec UMINUS { $$ = -$2; }
    | NUMBER               { $$ = $1; }
    ;
%%

int main() {
    printf("Enter expressions (Ctrl+D to exit):\n");
    return yyparse();
}

int yyerror(const char* s) {
    printf("Parse error: %s\n", s);
    return 1;
}

