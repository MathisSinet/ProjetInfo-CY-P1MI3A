#Core variables

DIR = src
BUILD = build
SAVEDIR = saves
EXEC = $(BUILD)/CosmicYonder
CC = gcc

#Soucre list

SRC = $(wildcard $(DIR)/*.c)
HEADERS = $(wildcard $(DIR)/*.h)
OBJ = $(patsubst $(DIR)/%.c,$(BUILD)/%.o,$(SRC))

#Compiler flags

CFLAGS = -Wall -lncursesw

#Targets

all: $(EXEC)

$(BUILD)/%.o: $(DIR)/%.c $(HEADERS) | $(BUILD)
	$(CC) -c $< -o $@ $(CFLAGS)

$(EXEC): $(OBJ) | $(SAVEDIR)
	$(CC) $^ -o $@ $(CFLAGS)

$(BUILD):
	mkdir -p $(BUILD)/
$(SAVEDIR):
	mkdir -p $(SAVEDIR)/

#Clean the build directory

clean:
	rm -rf $(BUILD)
	rm -rf $(SAVEDIR)

#Touch every source file

touch:
	touch src/*

#Run the program

run: $(EXEC)
	$(EXEC)