all:
	yacc --defines=./include/silc.h  --output=./src/y.tab.c ./src/silc.y
	lex -o ./src/silc.l.c ./src/silc.l
	gcc ./src/silc.l.c ./src/node.c ./src/register.c ./src/y.tab.c  ./src/evaluators.c -I ./include -o silc
