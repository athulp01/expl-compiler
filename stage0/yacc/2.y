%{
#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
%}

%token CHAR DIGIT

%%
variable: CHAR
        | variable CHAR
        | variable DIGIT
        | variable '\n' {printf("Valid variable");}
        | variable ' ' {printf("Valid variable");}
        ;
%%

int yyerror() {
    printf("invalid variable");
}

int yylex() {
    char c = getchar();
    if(isdigit(c)) return DIGIT;
    if(isalpha(c)) return CHAR;
    else return c;
}

int main() {
    yyparse();
    return 1;
}
