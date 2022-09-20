CC=gcc
CFLAGS= -Wall -Wextra -Werror -I.
DEPS = main.h
OBJ = func.o readline.o blockchain.o main.c
TARGET = my_blockchain
%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

fclean:
	rm -f ${TARGET} *.o
