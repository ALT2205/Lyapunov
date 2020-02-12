//
// Created by Antoine on 07/02/2020.
//

#ifndef LYAPUNOV_WINDOWMANAGER_H
#define LYAPUNOV_WINDOWMANAGER_H

#include <SDL2/SDL.h>
#include <exception>
#include <iostream>
#include <vector>

class WindowManager {

    unsigned int m_width, m_height;
    bool m_quit;
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    SDL_Texture* m_texture;


public:
    WindowManager(unsigned int w, unsigned int h);

    void eventLoop();

    void update(std::vector<Uint32>& pixels);

    ~WindowManager();
};


#endif //LYAPUNOV_WINDOWMANAGER_H
