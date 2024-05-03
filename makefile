#Core variables

DIR = src
BUILD = build
EXEC = $(BUILD)/CosmicYonder
CC = gcc

#Soucre list

SRC = $(wildcard $(DIR)/*.c)
OBJ = $(patsubst $(DIR)/%.c,$(BUILD)/%.o,$(SRC))

#Compiler flags

CFLAGS = -Wall -lncursesw

#Targets

all: $(EXEC)

$(BUILD)/%.o: $(DIR)/%.c $(DIR)/%.h $(DIR)/settings.h | $(BUILD)
	$(CC) -c $< -o $@ $(CFLAGS)

$(EXEC): $(OBJ)
	$(CC) $^ -o $@ $(CFLAGS)

$(BUILD):
	mkdir -p $(BUILD)/

#Clean the build directory

clean:
	rm -rf $(BUILD)

#Touch every source file

touch:
	touch src/*

#Run the program

run: $(EXEC)
	$(EXEC)