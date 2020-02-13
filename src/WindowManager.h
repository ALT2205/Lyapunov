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

    SDL_Rect m_windowPosition;
    bool m_quit;
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    SDL_Texture* m_texture;
    SDL_Rect m_texturePosition;

public:
    WindowManager(unsigned int w, unsigned int h);

    void initRender(SDL_Rect texturePosition);

    SDL_Rect& getTexturePosition();

    virtual void eventLoop();

    virtual void onResized(unsigned int newWidth, unsigned int newHeight);

    void update(std::vector<Uint32>& pixels);

    void setTexturePosition(SDL_Rect texturePosition);

    const SDL_Rect& getWindowPosition() const;

    ~WindowManager();
};


#endif //LYAPUNOV_WINDOWMANAGER_H
