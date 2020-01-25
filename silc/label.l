%option noyywrap
%{ 
    #include <stdlib.h>
	#include <stdio.h>
    int mem = 2054;
%}

token [a-zA-Z0-9]+

%%
"\n"    {mem += 2;}
{token}":" {printf("%d, %s", mem+2, yytext);}

%%

int main() {
    yyin = fopen("../out.xsm", "r");
    yylex();
    return 1;
}
 