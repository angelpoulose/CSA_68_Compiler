%{
#include "y.tab.h"
#include <stdlib.h>
%}

%%

"for"                       { return FOR; }
"++"                        { return INCR; }
"--"                        { return DECR; }
"=="|"!="|"<="|">="|"<"|">" { return RELOP; }
[0-9]+                      { yylval = atoi(yytext); return NUM; }
[a-zA-Z_][a-zA-Z0-9_]*      { return ID; }

"="                         { return '='; }
";"                         { return ';'; }
"("                         { return '('; }
")"                         { return ')'; }
"{"                         { return '{'; }
"}"                         { return '}'; }

[ \t\n]                     ; // ignore whitespace

.                           { return yytext[0]; }

%%

