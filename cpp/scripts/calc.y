%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <linux/version.h>

extern int yylex();
extern void yyerror(const char *s);

// 存储变量的值
#define MAX_VARIABLES 100
char *variables[MAX_VARIABLES];
float values[MAX_VARIABLES];
int var_count = 0;

// 查找变量索引
int find_variable(const char *name) {
    for (int i = 0; i < var_count; i++) {
        if (strcmp(variables[i], name) == 0) {
            return i;
        }
    }
    return -1; // 未找到
}

// 设置变量值
void set_variable(const char *name, float value) {
    int index = find_variable(name);
    if (index == -1) {
        if (var_count >= MAX_VARIABLES) {
            yyerror("变量数量超过最大限制");
            return;
        }
        variables[var_count] = strdup(name);
        values[var_count] = value;
        var_count++;
    } else {
        values[index] = value;
    }
}

// 获取变量值
float get_variable(const char *name) {
    int index = find_variable(name);
    if (index == -1) {
        yyerror("使用未定义的变量");
        return 0;
    }
    return values[index];
}

%}

%union {
    float fval;
    char *sval;
}

%token <fval> NUMBER
%token <sval> IDENTIFIER
%token PLUS MINUS MULTIPLY DIVIDE POWER ASSIGN
%token LPAREN RPAREN

%type <fval> expression
%type <fval> term
%type <fval> factor

%left PLUS MINUS
%left MULTIPLY DIVIDE
%right POWER
%nonassoc UMINUS

%%
program:
    program statement '\n'      { }
    | /* 空规则 */
    ;

statement:
    expression                  { printf("Result: %.2f\n", $1); }
    | IDENTIFIER ASSIGN expression {
        set_variable($1, $3);
        printf("%s = %.2f\n", $1, $3);
    }
    ;

expression:
    expression PLUS term        { $$ = $1 + $3; }
    | expression MINUS term     { $$ = $1 - $3; }
    | term                      { $$ = $1; }
    ;

term:
    term MULTIPLY factor        { $$ = $1 * $3; }
    | term DIVIDE factor {
        if ($3 == 0) {
            yyerror("除以零错误");
            $$ = 0;
        } else {
            $$ = $1 / $3;
        }
    }
    | factor                    { $$ = $1; }
    ;

factor:
    factor POWER factor         { $$ = pow($1, $3); }
    | MINUS factor %prec UMINUS { $$ = -$2; }
    | LPAREN expression RPAREN  { $$ = $2; }
    | IDENTIFIER {
        $$ = get_variable($1);
    }
    | NUMBER                    { $$ = $1; }
    | IDENTIFIER LPAREN expression RPAREN {
        if (strcmp($1, "sin") == 0) {
            $$ = sin($3);
        } else if (strcmp($1, "cos") == 0) {
            $$ = cos($3);
        } else if (strcmp($1, "sqrt") == 0) {
            if ($3 < 0) {
                yyerror("负数无法开平方");
                $$ = 0;
            } else {
                $$ = sqrt($3);
            }
        } else {
            yyerror("未定义的函数");
            $$ = 0;
        }
    }
    ;

%%
void yyerror(const char *s) {
    fprintf(stderr, "错误: %s\n", s);
}

int main(int argc, char **argv) {
    printf("欢迎使用扩展的计算器！支持变量、幂运算和函数\n");
    printf("输入示例: x = 2 + 3, y = x^2, sin(3.14), y + 1\n");
    printf("按 Ctrl+D 退出\n\n");
    yyparse();
    return 0;
}

