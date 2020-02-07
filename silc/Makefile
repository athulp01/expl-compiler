all:
	yacc --defines=./include/silc.h  --output=./src/y.tab.c ./src/silc.y
	flex -o ./src/silc.l.c ./src/silc.l
	gcc -g ./src/datastructures.c  ./src/silc.l.c  ./src/y.tab.c  ./src/evaluators.c -I ./include -o silc
