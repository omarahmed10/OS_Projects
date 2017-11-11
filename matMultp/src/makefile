program_NAME := matmult
CC := gcc
CFLAGS := -c

all: $(program_NAME)

$(program_NAME): matmult.o
	$(CC) matmult.o -o $(program_NAME) -lpthread

matmult.o: matmult.c
	$(CC) $(CFLAGS) matmult.c

clean:
	rm -rf *o c_* $(program_NAME)
