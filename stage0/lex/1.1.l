%option noyywrap
%{
#include<stdio.h>

int lines = 0, words = 0, chars = 0;

%}

line [\n]
word [a-zA-Z]*
space [' ']*

%%
{line} {lines++;}
{word} {words++;chars += (yyleng);}
{space} {}
%%

int main() {
    yylex();
    printf("\nNumber of lines is %d\n", lines);
    printf("Number of words is %d\n", words);
    printf("Number of chars is %d\n", chars);
    return 1;
}

