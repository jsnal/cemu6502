CC = gcc
CFLAGS = -Wall -g -std=c99
NAME = all

SOURCES = \
					src/cpu.c \
					src/handlers.c \
					src/machine.c \
					src/memory.c \

OBJECTS = ${SOURCES:.c=.o}

$(NAME): src/main.o $(OBJECTS)
	$(CC) $(CFLAGS) src/main.o $(OBJECTS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

refresh:
	find . -name -prune -o \( -name '*.c' -or -name '*.h' -or -name 'Makefile' \) | sort > cemu6502.files

.PHONY: clean
clean:
	rm -f $(NAME) $(OBJECTS)
