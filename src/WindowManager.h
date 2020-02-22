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
    SDL_Texture* m_draw;
    SDL_Texture* m_texture;
    SDL_Rect m_texturePosition;
    SDL_Rect m_textureOriginalSize;

public:
    WindowManager(unsigned int w, unsigned int h);

    void initRender(SDL_Rect size, SDL_Rect position);

    void drawRect(int x, int y, int w, int h);

    void updateTexture(std::vector<Uint32>& pixels) const;

    void blitTexture() const;

    void updateScreen() const;

    virtual void onResized(unsigned int newWidth, unsigned int newHeight) = 0;

    virtual void onMouseClick(unsigned int x, unsigned int y) = 0;

    virtual void onMouseMove(unsigned int x, unsigned int y) = 0;

    virtual void onMouseWheel() = 0;

    virtual void onKeyboard(int c) = 0;

    virtual void onTick() = 0;

    void eventLoop();

    const SDL_Rect& getTexturePosition() const;

    void setTexturePosition(SDL_Rect texturePosition);

    ~WindowManager();

};

std::ostream& operator<<(std::ostream& flux, SDL_Rect rect);

#endif //LYAPUNOV_WINDOWMANAGER_H
