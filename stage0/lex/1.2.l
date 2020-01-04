%option noyywrap
%{
#include<stdio.h>

int num_int, num_float;

%}

integer [0-9]*
float [0-9]*[\.][0-9]*

%%
{integer} {num_int++;}
{float} {num_float++;}
%%

int main() {
    yylex();
    printf("\nNumber of integers is %d\n", num_int);
    printf("Number of floats is %d\n", num_float);
}
