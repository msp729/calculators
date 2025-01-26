CC = clang

CCFLAGS = -std=c11 -Wall -Werror -fshort-enums -fsanitize=address

main: bin/main

tests: bin/complex bin/num

bin/%: src/%.c
	mkdir -p bin
	$(CC) $(CCFLAGS) $< -o $@

clean:
	rm bin/*
