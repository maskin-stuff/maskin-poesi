CFLAGS=-Wall -Wextra -Wpedantic

SRC=$(wildcard *.c)
OBJ=$(SRC:%.c=%.o)

poem: $(OBJ)
	gcc -o $@ $^

%.o: %.c
	gcc $(CFLAGS) -o $@ -c $<

clean:
	rm $(OBJ) poem

.PHONY:
	clean
