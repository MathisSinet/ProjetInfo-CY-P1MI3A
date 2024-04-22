#Core variables

DIR = src
BUILD = build
EXEC = $(BUILD)/CosmicYonder

#Soucre list

SRC = $(wildcard src/*.c)
OBJ = $(patsubst $(DIR)/%.c,$(BUILD)/%.o,$(SRC))

#Compiler flags

CFLAGS = -Wall

#Targets

all: $(EXEC)

$(BUILD)/%.o: $(DIR)/%.c $(DIR)/%.h $(DIR)/settings.h | $(BUILD)
	gcc $(CFLAGS) -c $< -o $@

$(EXEC): $(OBJ)
	gcc $(CFLAGS) $^ -o $@

$(BUILD):
	mkdir -p $(BUILD)/

clean:
	rm -rf $(BUILD)

run: $(EXEC)
	$(EXEC)