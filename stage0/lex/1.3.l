%option noyywrap
%{
#include<stdio.h>
#include<ctype.h>
%}

pattern [A][a-zA-Z]{2}
word [a-zA-Z]*

%%
{pattern} {for(int i=0; i<yyleng;i++) printf("%c",toupper(yytext[i]));printf("\n");}
{word} {}
%%

int main() {
    yylex();
    return 1;
}
