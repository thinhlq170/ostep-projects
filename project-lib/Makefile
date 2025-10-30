CC = gcc
CFLAGS = -Wall -Werror

sources = exception/assert.c exception/except.c mem/mem.c
objects = assert.o except.o mem.o

all: $(objects)

$(objects): $(sources)
	$(CC) $(CFLAGS) -c $^


clean:
	rm -f $(objects)