FLAGS = -Wall -Wextra -g -std=c++0x -pthread -fdump-tree-optimized
SFML_LIBS = -lsfml-graphics -lsfml-window -lsfml-system
SDL_LIBS = -lSDL2
CC = g++
BUILD_DIR=build
SRC_SIR=src
.PHONY: clean

all: dir lyapunov

dir:
	@if [ ! -d "build" ]; then mkdir build; fi

lyapunov: $(BUILD_DIR)/Time.o $(BUILD_DIR)/WindowManager.o $(BUILD_DIR)/Lyapunov.o $(BUILD_DIR)/Region.o
	$(CC) -o $@ $^ $(FLAGS) $(SDL_LIBS)

$(BUILD_DIR)/Time.o: $(SRC_SIR)/Time.cpp
	$(CC) -c -o $@ $^ $(FLAGS)

$(BUILD_DIR)/WindowManager.o: $(SRC_SIR)/WindowManager.cpp
	$(CC) -c -o $@ $^ $(FLAGS)

$(BUILD_DIR)/Lyapunov.o: $(SRC_SIR)/Lyapunov.cpp
	$(CC) -c -o $@ $^ $(FLAGS)

$(BUILD_DIR)/Region.o: $(SRC_SIR)/Region.cpp
	$(CC) -c -o $@ $^ $(FLAGS)

clean:
	rm -Rf $(BUILD_DIR)/*.o $(SRC_SIR)/*.gch