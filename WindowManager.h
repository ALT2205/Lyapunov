//
// Created by Antoine on 07/02/2020.
//

#ifndef LYAPUNOV_WINDOWMANAGER_H
#define LYAPUNOV_WINDOWMANAGER_H

#include <SDL2/SDL.h>
#include <exception>


class WindowManager {

    int width, height;
    bool quit;
    Uint32* pixels;
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;


public:
    WindowManager(int width, int height);

    void eventLoop();

    void setPixels(Uint32* pixels);

    ~WindowManager();

    void update();
};


#endif //LYAPUNOV_WINDOWMANAGER_H
