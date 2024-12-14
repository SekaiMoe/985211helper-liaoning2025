%{
#include <stdio.h>
#include <stdlib.h>

void execute_command(const char *cmd);

%}

%union {
    char *sval;
}

%token <sval> STRING
%token OPERATION NEWLINE

%%
commands:
    /* 空规则 */
    | commands command
    ;

command:
    OPERATION STRING NEWLINE {
        printf("执行命令: %s\n", $2);
        execute_command($2);
        free($2);
    }
    ;

%%
void execute_command(const char *cmd) {
    int ret = system(cmd);
    if (ret != 0) {
        fprintf(stderr, "命令执行失败: %s\n", cmd);
        exit(1);
    }
}

int main() {
    printf("读取配置并执行命令...\n");
    yyparse();
    return 0;
}

void yyerror(const char *s) {
    fprintf(stderr, "错误: %s\n", s);
}

