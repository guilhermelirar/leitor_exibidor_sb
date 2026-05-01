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

obj/%.o: src/%.c include/optable.def
	@mkdir -p obj
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@


clean:
	rm -rf obj bin

run: $(BIN)
	./$(BIN)

debug: $(BIN)
	gdb --args ./$(BIN) referencia/Teste.class

valgrind: $(BIN)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(BIN) referencia/Teste.class

.PHONY: all clean run debug valgrind
