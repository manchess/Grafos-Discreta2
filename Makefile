CC = gcc
CFLAGS = -Wall -Wextra -Werror -Waddress -Wattributes -std=c99 -O3 -g
OBJ = Grafo.o

all: $(OBJ)
	$(CC) $(CFLAGS) -o main $(OBJ)
clean:
	$(RM) $(OBJ) main

