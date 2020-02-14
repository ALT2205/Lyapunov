//
// Created by Antoine on 07/02/2020.
//

#include "WindowManager.h"

//Afficher SDL_Rect
std::ostream& operator<<(std::ostream& flux, SDL_Rect rect){
    flux << "x=" << rect.x << ", y=" << rect.y << ", w=" << rect.w << ", h=" << rect.h << " ";
    return flux;
}

WindowManager::WindowManager(unsigned int w, unsigned int h)
        : m_windowPosition(), m_quit(false), m_window(nullptr), m_renderer(nullptr), m_texture(nullptr),
          m_texturePosition(), m_textureOriginalSize{}{
    if(SDL_Init(SDL_INIT_VIDEO) != 0){
        throw std::runtime_error(SDL_GetError());
    }
    m_window = SDL_CreateWindow("Fractales de Lyapunov", SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED, (int) w, (int) h, SDL_WINDOW_RESIZABLE);
    if(m_window == nullptr){
        throw std::runtime_error(SDL_GetError());
    }
    m_windowPosition.w = (int) w;
    m_windowPosition.h = (int) h;
}

/*
 * Initialise une Texture et un Render
 * La texture est au format RGB, et a une taille de size.w * size.h, à la position position
 * */
void WindowManager::initRender(SDL_Rect size, SDL_Rect position){
    m_renderer = SDL_CreateRenderer(m_window, -1, 0);
    if(m_renderer == nullptr){
        throw std::runtime_error(SDL_GetError());
    }
    m_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STATIC, size.w, size.h);
    if(m_texture == nullptr){
        throw std::runtime_error(SDL_GetError());
    }
    m_texturePosition = position;
    m_textureOriginalSize = size;
}

SDL_Rect& WindowManager::getTexturePosition(){
    return m_texturePosition;
}

SDL_Rect& WindowManager::getOriginalSize(){
    return m_textureOriginalSize;
}

const SDL_Rect& WindowManager::getWindowPosition() const{
    return m_windowPosition;
}

/*
 * Mets à jour les pixels d'une texture
 * */
void WindowManager::update(std::vector<Uint32>& pixels) const{
    SDL_UpdateTexture(m_texture, nullptr, pixels.data(), (int) (m_textureOriginalSize.h * sizeof(Uint32)));
    showTexture();
}

/*
 * Affiche une texture à l'écran
 * */
void WindowManager::showTexture() const{
    SDL_RenderClear(m_renderer);
    SDL_RenderCopy(m_renderer, m_texture, nullptr, &m_texturePosition);
    SDL_RenderPresent(m_renderer);
}

void WindowManager::setTexturePosition(SDL_Rect texturePosition){
    m_texturePosition = texturePosition;
}

void WindowManager::eventLoop(){
    SDL_Event event;
    while(!m_quit){
        SDL_WaitEvent(&event);
        switch(event.type){
            case SDL_WINDOWEVENT:
                switch(event.window.event){
                    case SDL_WINDOWEVENT_RESIZED:
                        onResized(event.window.data1, event.window.data2);
                        break;
                }
                break;
            case SDL_QUIT:
                m_quit = true;
                break;

        }
    }
}

WindowManager::~WindowManager(){
    SDL_DestroyTexture(m_texture);
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}













