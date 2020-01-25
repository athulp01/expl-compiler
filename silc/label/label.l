%{ 
    #include <stdlib.h>
	#include <stdio.h>
    #include <string.h>

    #define LABEL_LENGTH 10

    int curMem = 2054, pass = 1;
    FILE *translated;

    struct labelNode {
        char *label;
        int mem;
        struct labelNode* next;
    };
    struct labelNode* table, *head;
    struct labelNode* createLabel(char *name, int addr) {
        struct labelNode* tmp = (struct labelNode*) malloc(sizeof(struct labelNode));
        tmp->label = name;
        tmp->mem = addr;
        tmp->next = NULL;
        return tmp;
    }
%}

label ^[a-zA-Z0-9]+[:](\n)
branch1 ("JZ"|"JNZ")[' ']+[R][0-9]+[,][' ']*[a-zA-Z0-9]+
branch2 ("JMP")[' ']+[a-zA-Z0-9]+

%%
[\n]    {curMem += 2;}
{label} {
    if(pass == 1) {
            char *label = (char*)malloc(sizeof(char)*LABEL_LENGTH);
            int i;
            for(i=0; i<yyleng-2; i++) {
                label[i] = yytext[i];
            }
            label[i] = '\0';
            if(table) {
                table->next = createLabel(label, curMem+2-16);
                table = table->next;
            }else {
                table = createLabel(label, curMem+2-16);
                head = table;
            }
        }
    }

{branch1} {
            if(pass == 2) {
                char *inst = yytext;
                int i;
                //print excluding the label
                for(i=0; i<yyleng; i++) {
                    if(inst[i] != ',') fprintf(translated, "%c", inst[i]);
                    else break;
                }

                struct labelNode* tmp = head;
                while(tmp) {
                    if(strcmp(tmp->label, yytext+i+2) == 0) {
                        fprintf(translated, ", %d\n", tmp->mem);
                        break;
                    }
                    tmp = tmp->next;
                }

            }
}

{branch2} {
            if(pass == 2) {
                int i;
                for(i=0; i<yyleng; i++) {
                    if(yytext[i] != ' ') fprintf(translated, "%c", yytext[i]);
                    else break;
                }
                struct labelNode* tmp = head;
                while(tmp) {
                    if(strcmp(tmp->label, yytext+i+1) == 0) {
                        fprintf(translated, ", %d\n", tmp->mem);
                        break;
                    }
                    tmp = tmp->next;
                }

            }
}

(.)*  {if(pass == 2)fprintf(translated, "%s\n", yytext);}

%%

int yywrap() {
    if(pass == 1) {
        pass++;
        yyin = fopen("../out.xsm", "r");
        return 0;
    }
    return 1;
}
int main() {
    head = NULL;table = NULL;
    translated = fopen("translated.xsm", "w");
    yyin = fopen("../out.xsm", "r");
    yylex();

    return 1;
}