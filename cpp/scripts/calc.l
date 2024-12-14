%{
#include "y.tab.h"
%}

// 定义空白字符（忽略它们）
%option noyywrap

%%
[0-9]+(\.[0-9]+)?  { yylval.fval = atof(yytext); return NUMBER; }
"+"                { return PLUS; }
"-"                { return MINUS; }
"*"                { return MULTIPLY; }
"/"                { return DIVIDE; }
"("                { return LPAREN; }
")"                { return RPAREN; }
[ \t\n]            { /* 忽略空白字符 */ }
.                  { printf("非法字符: %s\n", yytext); return yytext[0]; }
%%


