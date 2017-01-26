CC = gcc
CFLAGS = -O3 -Wall 
CFLAGS += -D_LARGEFILE_SOURCE
CFLAGS += -fno-exceptions
CFLAGS += -finline-functions
CFLAGS += -funroll-loops
CFLAGS += -D_FILE_OFFSET_BITS=64
 
output: record.o
	gcc record.o -o output

record.o: record.c record.h
	gcc -c record.c

clean:
	rm *.o output
  
 