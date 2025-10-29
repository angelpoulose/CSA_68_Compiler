%{
#include "y.tab.h"
%}

%%

[a-zA-Z][a-zA-Z0-9]*   { yylval.str = strdup(yytext); return IDENTIFIER; }
.                      { return yytext[0]; }
\n                     { return 0; }

%%



