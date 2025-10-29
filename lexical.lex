%{

#include <stdio.h>

%}

%option noyywrap

/* Define regular expressions for different token types */

DIGIT      [0-9]

LETTER     [a-zA-Z]

IDENTIFIER {LETTER}({LETTER}|{DIGIT})*

NUMBER     {DIGIT}+

WHITESPACE [ \t\n]+

COMMENT1    "//".*

COMMENT2  "/*"([^*]|\*+[^/*])*\*+"/"

%%


{WHITESPACE}        { /* Ignore whitespace */ }

{COMMENT1}           { /* Ignore comments */ }

{COMMENT2}           { /* Ignore comments */ }

"int"               { printf("TOKEN_INT\n"); }

"return"            { printf("TOKEN_RETURN\n"); }

"if"                { printf("TOKEN_IF\n"); }

"else"              { printf("TOKEN_ELSE\n"); }

"while"             { printf("TOKEN_WHILE\n"); }

"for"               { printf("TOKEN_FOR\n"); }

"="                 { printf("TOKEN_ASSIGN\n"); }

"=="                { printf("TOKEN_EQ\n"); }

"!="                { printf("TOKEN_NEQ\n"); }

"<"                 { printf("TOKEN_LT\n"); }

"<="                { printf("TOKEN_LE\n"); }

">"                 { printf("TOKEN_GT\n"); }

">="                { printf("TOKEN_GE\n"); }

"+"                 { printf("TOKEN_PLUS\n"); }

"-"                 { printf("TOKEN_MINUS\n"); }

"*"                 { printf("TOKEN_MULT\n"); }

"/"                 { printf("TOKEN_DIV\n"); }

"("                 { printf("TOKEN_LPAREN\n"); }

")"                 { printf("TOKEN_RPAREN\n"); }

"{"                 { printf("TOKEN_LBRACE\n"); }

"}"                 { printf("TOKEN_RBRACE\n"); }

";"                 { printf("TOKEN_SEMICOLON\n"); }

{IDENTIFIER}        { printf("TOKEN_IDENTIFIER(%s)\n", yytext); }

{NUMBER}            { printf("TOKEN_NUMBER(%s)\n", yytext); }

.                   { printf("TOKEN_UNKNOWN(%s)\n", yytext); }

%%

int main(int argc, char **argv) {

    if (argc > 1) {

        FILE *file = fopen(argv[1], "r");

        if (!file) {

            fprintf(stderr, "Could not open %s\n", argv[1]);

            return 1;

        }

        yyin = file;

    }

    yylex();

    return 0;
}
