%{
#include<stdio.h>
#include<stdlib.h>

int nested;
char before = '\0';
%}

%right '{'
%left '}'
%token IF

%%
if : IF '{' '}' {nested = 1;}
   | if IF '{' '}' 
   | IF '{' if '}'     {nested++; printf("nested %d", nested);}
   | if IF '{' if '}' 
   ;
%%

int yylex() {
    char c = getchar();
    if(c == 'f' && before == 'i') {
        before = '\0';
        return IF;
    }else if(c == '{' || c == '}') {
        return c;
    }else {
        before = c;
        yylex();
    }
}

int yyerror(char const *s) {
    printf("yyerror %s", s);
}

int main() {
    yyparse();
    return 1;
}
