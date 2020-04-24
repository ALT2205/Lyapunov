//
// Created by Antoine on 07/02/2020.
//

#ifndef LYAPUNOV_WINDOWMANAGER_H
#define LYAPUNOV_WINDOWMANAGER_H

#include <SDL2/SDL.h>
#include <exception>
#include <iostream>
#include <vector>

// Cette classe permet de s'occuper de tout ce qui est nécessaire
// à l'affichage de la Fractale
class WindowManager {

    SDL_Rect m_windowPosition;          // Comprend les coordonnées de la fenêtre où est générée la fractale
    bool m_quit;                        // Détérmine si l'on a quitté la fenêtre
    SDL_Window* m_window;               // Pointeur sur la fenêtre de la fractale
    SDL_Renderer* m_renderer;           // Renderer qui s'occupe de la génération des fractales
    SDL_Texture* m_draw;                // TODO ????
    SDL_Texture* m_texture;             // Texture de la fractale
    SDL_Rect m_texturePosition;         // Position de la texture de la fractale
    SDL_Rect m_textureOriginalSize;     // Position Originale de la texture
    SDL_Rect m_mousePosition;           // Position du pointeur de la souris
    double angle = 0;
    SDL_RendererFlip m_flip = SDL_FLIP_NONE;

public:
    WindowManager(unsigned int w, unsigned int h);

    void initRender(SDL_Rect size, SDL_Rect position);

    void drawRect(int x, int y, int w, int h);

    void updateTexture(std::vector<Uint32>& pixels) const;

    void blitTexture() const;

    void updateScreen() const;

    const SDL_Rect& getMousePosition() const;

    void addDegree(int degrees);

    void rotateHorizontally();

    void rotateVertically();

    SDL_RendererFlip getFlip();

    void setFlip(SDL_RendererFlip flip);

    virtual void onResized(unsigned int newWidth, unsigned int newHeight) = 0;

    virtual void onMouseClick(unsigned int x, unsigned int y, unsigned int button) = 0;

    virtual void onMouseMove(unsigned int x, unsigned int y) = 0;

    virtual void onMouseWheel() = 0;

    virtual void onKeyboardUp(int c) = 0;

    virtual void onKeyboardDown(int c) = 0;

    virtual void onTick() = 0;

    void eventLoop();

    const SDL_Rect& getTexturePosition() const;

    void setTexturePosition(SDL_Rect texturePosition);

    ~WindowManager();

};

std::ostream& operator<<(std::ostream& flux, SDL_Rect rect);

#endif //LYAPUNOV_WINDOWMANAGER_H
