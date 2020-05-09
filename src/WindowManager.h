//
// Created by Antoine on 07/02/2020.
//

#ifndef LYAPUNOV_WINDOWMANAGER_H
#define LYAPUNOV_WINDOWMANAGER_H

#include <SDL2/SDL.h>
#include <exception>
#include <iostream>
#include <vector>
#include <iostream>
#include <fstream>
#include "Time.h"

// Cette classe permet de s'occuper de tout ce qui est nécessaire
// à l'affichage de la Fractale
class WindowManager {

    bool m_quit;                        // Détermine si l'on a quitté la fenêtre
    SDL_Window* m_window;               // Pointeur sur la fenêtre de la fractale
    SDL_Renderer* m_renderer;           // Renderer qui s'occupe de la génération des fractales
    SDL_Texture* m_texture;             // Texture de la fractale
    SDL_Rect m_texturePosition;         // Position de la texture de la fractale
    int m_pitch;                        // Hauteur de la fractale
    SDL_Rect m_mousePosition;           // Position du pointeur de la souris

public:
    WindowManager();

    void initRender(SDL_Rect size);

    void drawRect(int x, int y, int w, int h);

    void updateTexture(std::vector<Uint32>& pixels) const;

    void blitTexture() const;

    void updateScreen() const;

    void screenShot() const;

    const SDL_Rect& getMousePosition() const;

    virtual void onResized(unsigned int newWidth, unsigned int newHeight) = 0;

    virtual void onMouseClick(int x, int y, int button) = 0;

    virtual void onMouseMove(int x, int y) = 0;

    virtual void onMouseWheel(int amount) = 0;

    virtual void onKeyboardDown(int c) = 0;

    void eventLoop();

    const SDL_Rect& getTexturePosition() const;

    void setTexturePosition(SDL_Rect texturePosition);

    ~WindowManager();
};

std::ostream& operator<<(std::ostream& flux, SDL_Rect rect);

#endif //LYAPUNOV_WINDOWMANAGER_H
