DIR=src
BUILD=build

all: $(BUILD)/CosmicYonder

$(BUILD)/game.o: $(DIR)/game.c $(DIR)/settings.h $(DIR)/game.h $(BUILD)
	gcc -c $< -o $@

$(BUILD)/display.o: $(DIR)/display.c $(DIR)/display.h $(BUILD)
	gcc -c $< -o $@

$(BUILD)/main.o: $(DIR)/main.c $(BUILD)
	gcc -c $< -o $@

$(BUILD)/CosmicYonder: $(BUILD)/game.o $(BUILD)/display.o  $(BUILD)/main.o
	gcc $^ -o $@

$(BUILD):
	mkdir -p $(BUILD)/

clean:
	cd $(BUILD)/; rm *.o; cd ..

run:
	cd $(BUILD); ./CosmicYonder