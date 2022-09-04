TARGET = my_blockchain
WWW = -Wall -Wextra -Werror
CC = gcc
SRC = main.c func_and_time.c readline.c
all:
	${CC} ${WWW} -o ${TARGET} ${SRC}

fclean:
	rm -f ${TARGET}
