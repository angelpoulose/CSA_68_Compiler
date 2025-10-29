%{
#include "y.tab.h"
#include <stdio.h>
#include <stdlib.h>
%}

%%

[0-9]+        { yylval = atoi(yytext); return NUMBER; }
[ \t\r]+      ;  // Ignore spaces and tabs (but NOT \n)
\n            { return '\n'; }  // ✅ Explicitly return newline to parser
"+"           { return PLUS; } 
"-"           { return MINUS; }
"*"           { return MUL; }
"/"           { return DIV; }
"("           { return LPAREN; }
")"           { return RPAREN; }

.             { printf("Invalid character: %s\n", yytext); }

%%

int yywrap() {
    return 1;
}

