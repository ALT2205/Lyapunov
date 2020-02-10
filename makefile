FLAGS = -Wall -Wextra -g
SFML_LIBS = -lsfml-graphics -lsfml-window -lsfml-system
SDL_LIBS = -lSDL2
CC = g++
.PHONY: clean

all: lyapunov

lyapunov:  WindowManager.o Lyapunov.o
	$(CC) -o $@ $^ $(FLAGS) $(SDL_LIBS)

WindowManager.o: WindowManager.h WindowManager.cpp
	$(CC) -c $^ $(FLAGS)

Lyapunov.o: Lyapunov.cpp
	$(CC) -c $^ $(FLAGS)

clean:
	rm -Rf *.o headers/*.gch