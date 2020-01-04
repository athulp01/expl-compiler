%option noyywrap
%{
    #include<stdio.h>
%}

number [0-9]+

%%
{number} {printf(" number");}
%%

int main() {
    yylex();
    return 1;
}

