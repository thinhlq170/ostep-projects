CC = gcc
CFLAGS = -Wall -Werror

sources = wzip.c
objects = zipentry.o ../../common.o $(wildcard ../../project-lib/*.o)
target = wzip

all: $(objects) 
	$(CC) $(CFLAGS) $^ $(sources) -o $(target)

zipentry.o: zipentry.c
	$(CC) $(CFLAGS) -c $^ -o $@


clean:
	rm -f $(wildcard *.o) $(target)