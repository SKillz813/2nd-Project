# Makefile

```makefile
# Ορισμός μεταβλητών
CC = gcc
CFLAGS = -Wall -Wextra -g
SRC = src/main.c src/functions.c
OBJ = $(SRC:.c=.o)
EXEC = build/eshop

# Κανόνας μεταγλώττισης
all: $(EXEC)

$(EXEC): $(OBJ)
	mkdir -p build
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Κανόνας καθαρισμού
clean:
	rm -rf $(OBJ) $(EXEC)

.PHONY: all clean

