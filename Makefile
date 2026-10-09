main: main.c lexer.c parser.c symbol_table.c debug.c
	gcc -ggdb -o main main.c lexer.c parser.c symbol_table.c debug.c

run: main
	./main

clean:
	rm -f main
