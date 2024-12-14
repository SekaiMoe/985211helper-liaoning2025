%{
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// 存储词法分析器返回的值
extern int yylex();
extern int yyparse();
extern FILE *yyin;
void yyerror(const char *s);
%}

// 定义 tokens
%token NUMBER
%token PLUS MINUS MULTIPLY DIVIDE LPAREN RPAREN

%left PLUS MINUS
%left MULTIPLY DIVIDE
%nonassoc UMINUS

%union {
    float fval;
}

%type <fval> expression

%%
program:
    program expression '\n'    { printf("Result: %.2f\n", $2); }
    | program '\n'
    | /* 空规则 */
    ;

expression:
    expression PLUS expression   { $$ = $1 + $3; }
    | expression MINUS expression { $$ = $1 - $3; }
    | expression MULTIPLY expression { $$ = $1 * $3; }
    | expression DIVIDE expression {
        if ($3 == 0) {
            yyerror("除以零错误");
            $$ = 0;
        } else {
            $$ = $1 / $3;
        }
    }
    | LPAREN expression RPAREN  { $$ = $2; }
    | MINUS expression %prec UMINUS { $$ = -$2; }
    | NUMBER                   { $$ = $1; }
    ;

%%
void yyerror(const char *s) {
    fprintf(stderr, "错误: %s\n", s);
}

int main(int argc, char **argv) {
    if (argc > 1) {
        FILE *file = fopen(argv[1], "r");
        if (!file) {
            perror("打开文件失败");
            return 1;
        }
        yyin = file;
    }
    yyparse();
    return 0;
}

