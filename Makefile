main: src
	gcc -ggdb -Iinclude -o main src/*.c

run: main
	./main

config_test: src include tests
	gcc -ggdb -Iinclude -o config tests/config_test.c src/config.c src/debug.c src/lexer.c src/ebnf_parser.c src/symbol_table.c

run_config: config_test
	./config

clean:
	rm -f main config
