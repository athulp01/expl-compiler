%{ 
    #include <stdlib.h>
	#include <stdio.h>
    #include <string.h>

    #define LABEL_LENGTH 50

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

label [a-zA-Z0-9._]+[:](\n)
branch1 ("JZ"|"JNZ")[' ']+[R][0-9]+[,][' ']*[a-zA-Z0-9._]+
branch2 ("JMP")[' ']+[a-zA-Z0-9._]+
branch3 ("CALL")[' '][a-zA-Z._]+
virtual ("MOV R19, ")[a-zA-Z0-9._]+
start ("START")

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
                int found = 0;
                while(tmp) {
                    if(strcmp(tmp->label, yytext+i+1) == 0) {
                        fprintf(translated, " %d\n", tmp->mem);
                        found = 1;
                        break;
                    }
                    tmp = tmp->next;
                }
                if(!found) {
                    printf("Label not found\n");
                    exit(1);
                }

            }
}

{branch3} {

            if(pass == 2) {
                fprintf(translated, "CALL");
                int found = 0;
                struct labelNode* tmp = head;
                while(tmp) {
                    if(strcmp(tmp->label, yytext+5) == 0) {
                        fprintf(translated, " %d\n", tmp->mem);
                        found = 1;
                        break;
                    }
                    tmp = tmp->next;
                }
                if(!found) {
                    printf("Label not found\n");
                    exit(1);
                }
            }
}
{start} {

            if(pass == 2) {
                int found = 0;
                struct labelNode* tmp = head;
                while(tmp) {
                    if(strcmp(tmp->label, "main") == 0) {
                        fprintf(translated, "%d\n", tmp->mem);
                        found = 1;
                        break;
                    }
                    tmp = tmp->next;
                }
                if(!found) {
                    printf("Label not found\n");
                    exit(1);
                }
            }
}
{virtual} {

            if(pass == 2) {
                int found = 0;
                struct labelNode* tmp = head;
                while(tmp) {
                    if(strcmp(tmp->label, yytext+9) == 0) {
                        char *front = strtok(yytext, ",");
                        fprintf(translated, "%s, %d\n", front, tmp->mem);
                        found = 1;
                        break;
                    }
                    tmp = tmp->next;
                }
                if(!found) {
                    printf("Label not found\n");
                    exit(1);
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
    translated = fopen("../translated.xsm", "w");
    yyin = fopen("../out.xsm", "r");
    yylex();
    return 0;
}
