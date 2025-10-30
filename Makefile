CC = gcc
CFLAGS = -Wall -Werror

objects = common.o
sources = common.c

$(objects): $(sources)
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	rm -f $(objects)