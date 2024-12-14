%{
#include "y.tab.h"
%}

%%
\"[^\"]*\"              { yylval.sval = strdup(yytext + 1); yylval.sval[strlen(yylval.sval) - 1] = '\0'; return STRING; }
operation               { return OPERATION; }
\n                      { return NEWLINE; }
[ \t]+                  { /* 忽略空白 */ }
.                       { return yytext[0]; }
%%

