#Core variables

DIR = src
BUILD = build
EXEC = $(BUILD)/CosmicYonder
CC = gcc

#Soucre list

SRC = $(wildcard src/*.c)
OBJ = $(patsubst $(DIR)/%.c,$(BUILD)/%.o,$(SRC))

#Compiler flags

CFLAGS = -Wall -lncurses

#Targets

all: $(EXEC)

$(BUILD)/%.o: $(DIR)/%.c $(DIR)/%.h $(DIR)/settings.h | $(BUILD)
	$(CC) -c $< -o $@ $(CFLAGS)

$(EXEC): $(OBJ)
	$(CC) $^ -o $@ $(CFLAGS)

$(BUILD):
	mkdir -p $(BUILD)/

clean:
	rm -rf $(BUILD)

touch:
	touch src/*

run: $(EXEC)
	$(EXEC)