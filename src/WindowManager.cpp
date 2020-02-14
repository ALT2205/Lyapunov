//
// Created by Antoine on 07/02/2020.
//

#include "WindowManager.h"

WindowManager::WindowManager(unsigned int w, unsigned int h)
        : m_windowPosition(), m_quit(false), m_window(nullptr), m_renderer(nullptr), m_texture(nullptr), m_texturePosition(){
    if(SDL_Init(SDL_INIT_VIDEO) != 0){
        throw std::runtime_error(SDL_GetError());
    }
    m_window = SDL_CreateWindow("Fractales de Lyapunov", (unsigned int)SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, (int)w, (int)h, SDL_WINDOW_RESIZABLE);
    if(m_window == nullptr){
        throw std::runtime_error(SDL_GetError());
    }
    m_windowPosition.w = (int)w;
    m_windowPosition.h = (int)h;
}

void WindowManager::initRender(SDL_Rect texturePosition){
    m_renderer = SDL_CreateRenderer(m_window, -1, 0);
    if(m_renderer == nullptr){
        throw std::runtime_error(SDL_GetError());
    }
    m_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STATIC, texturePosition.w, texturePosition.h);
    if(m_texture == nullptr){
        throw std::runtime_error(SDL_GetError());
    }
    m_texturePosition = texturePosition;
}

SDL_Rect& WindowManager::getTexturePosition(){
    //SDL_Rect position = {m_texturePosition.x, m_texturePosition.y, 0, 0};
    //SDL_QueryTexture(m_texture, nullptr, nullptr, &position.w, &position.h);
    return m_texturePosition;
}

void WindowManager::update(std::vector<Uint32>& pixels){
    SDL_Rect position = getTexturePosition();
    SDL_UpdateTexture(m_texture, nullptr, pixels.data(), (int)(position.h * sizeof(Uint32)));
    SDL_RenderClear(m_renderer);
    SDL_RenderCopy(m_renderer, m_texture, nullptr, &position);
    SDL_RenderPresent(m_renderer);
}

void WindowManager::setTexturePosition(SDL_Rect texturePosition){
    SDL_RenderClear(m_renderer);
    SDL_RenderCopy(m_renderer, m_texture, nullptr, &texturePosition);
    SDL_RenderPresent(m_renderer);
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

const SDL_Rect& WindowManager::getWindowPosition() const{
    return m_windowPosition;
}

WindowManager::~WindowManager(){
    SDL_DestroyTexture(m_texture);
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}











