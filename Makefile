main: main.c lexer.c parser.c
	gcc -ggdb -o main main.c lexer.c parser.c

run: main
	./main

clean:
	rm -f main
