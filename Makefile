main: src
	gcc -ggdb -Iinclude -o main src/*.c

run: main
	./main

clean:
	rm -f main
