main: main.c
	gcc main.c -omain -g
main64: main.c
	gcc -march=native -Ofast main.c -omain -g
bst: bst.c
	gcc -Ofast bst.c -obst -g
run: main
	./main
d: main
	#gdb --args main -325 -916 -97 -905
	#gdb --args main 1 2 3
	gdb main
