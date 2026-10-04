main: main.c lexer.c parser.c debug.c
	gcc -ggdb -o main main.c lexer.c parser.c debug.c

run: main
	./main

clean:
	rm -f main
