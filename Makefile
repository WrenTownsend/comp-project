main: src
	gcc -ggdb -Iinclude -o main src/*.c

run: main
	./main

config_test: tests
	gcc -ggdb -Iinclude -o config tests/config_test.c src/config.c src/debug.c src/lexer.c src/ebnf_parser.c src/symbol_table.c

config_run: config_test
	./config

clean:
	rm -f main
