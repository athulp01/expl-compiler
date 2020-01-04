%option noyywrap
%{
#include<stdio.h>
%}

singleline ("//").*
quotes (\").*(\")
multiline ("/*")[a-zA-Z0-9 ' ' \n]*("*/")

%%
{singleline} {printf("\nSingle line comment - %s\n",yytext+2);}
{multiline} {printf("\nmulti line comment - %s\n",yytext+2);}
{quotes} {}
%%

int main() {
    yylex();
    return 1;
}
