CC      = gcc
CFLAGS  = -std=c11 -Wall -Wextra -g
INCLUDE = -Iinclude

SRC = $(wildcard src/*.c)
OBJ = $(SRC:src/%.c=obj/%.o)

BIN = bin/lexibid

all: $(BIN)

$(BIN): $(OBJ)
	@mkdir -p bin
	$(CC) $(OBJ) -o $(BIN)

obj/%.o: src/%.c
	@mkdir -p obj
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

clean:
	rm -rf obj bin

run: $(BIN)
	./$(BIN)

.PHONY: all clean run
