%{
#include "y.tab.h"
%}

%option noyywrap

%%

// 匹配数字（包括浮点数）
[0-9]+(\.[0-9]+)?    { yylval.fval = atof(yytext); return NUMBER; }

// 运算符
"+"                  { return PLUS; }
"-"                  { return MINUS; }
"*"                  { return MULTIPLY; }
"/"                  { return DIVIDE; }
"^"                  { return POWER; }
"="                  { return ASSIGN; }

// 括号
"("                  { return LPAREN; }
")"                  { return RPAREN; }

// 标识符（变量名或函数名）
[a-zA-Z_][a-zA-Z0-9_]* {
    yylval.sval = strdup(yytext);
    return IDENTIFIER;
}

// 空白字符（忽略）
[ \t\n]              { /* 忽略空白字符 */ }

// 未知字符
.                    { printf("非法字符: %s\n", yytext); return yytext[0]; }

%%

