DIR=src

all: $(DIR)/CosmicYonder

$(DIR)/game.o: $(DIR)/game.c $(DIR)/settings.h $(DIR)/game.h 
	gcc -c $< -o $@

$(DIR)/display.o: $(DIR)/display.c $(DIR)/display.h 
	gcc -c $< -o $@

$(DIR)/main.o: $(DIR)/main.c
	gcc -c $< -o $@

$(DIR)/CosmicYonder: $(DIR)/game.o $(DIR)/display.o  $(DIR)/main.o
	gcc $^ -o $@

clean:
	cd $(DIR); rm *.o; cd ..