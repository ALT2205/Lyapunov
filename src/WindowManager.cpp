//
// Created by Antoine on 07/02/2020.
//

#include "WindowManager.h"

// Surchage de l'opérateur <<. Permet d'afficher les champs d'un SDL_Rect
std::ostream& operator<<(std::ostream& flux, SDL_Rect rect){
    flux << "x=" << rect.x << ", y=" << rect.y << ", w=" << rect.w << ", h=" << rect.h << " ";
    return flux;
}

// Constructeur de la classe WindowMangager
WindowManager::WindowManager()
        : m_quit{false}, m_window{nullptr}, m_renderer{nullptr},
          m_texture{nullptr},
          m_texturePosition{}, m_pitch{}, m_mousePosition{}{
    if(SDL_Init(SDL_INIT_VIDEO) != 0){
        throw std::runtime_error(SDL_GetError());
    }
    m_window = SDL_CreateWindow("Fractales de Lyapunov", SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED, 0, 0, SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED);
    if(m_window == nullptr){
        throw std::runtime_error(SDL_GetError());
    }
}

// Initialise une Texture et un Render
// La texture est au format RGB, et a une taille de size.w * size.h, à la position position
void WindowManager::initRender(SDL_Rect size){
    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(m_renderer == nullptr){
        throw std::runtime_error(SDL_GetError());
    }
    m_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STATIC, size.w, size.h);
    if(m_texture == nullptr){
        throw std::runtime_error(SDL_GetError());
    }
    m_pitch = size.h;
}

// Permet de dessiner un rectangle à la positon X,Y de largeur et hauteur W,H
void WindowManager::drawRect(int x, int y, int w, int h){
    SDL_Rect rect = {x, y, w, h};
    SDL_RenderDrawRect(m_renderer, &rect);
}

//Mets à jour les pixels d'une texture
void WindowManager::updateTexture(std::vector<Uint32>& pixels) const{
    SDL_UpdateTexture(m_texture, nullptr, pixels.data(), (int) (m_pitch * sizeof(Uint32)));
}

// Réinitialise le Render avec uniquement la texture
void WindowManager::blitTexture() const{
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    SDL_RenderClear(m_renderer);
    SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
    SDL_RenderCopy(m_renderer, m_texture, nullptr, &m_texturePosition);
}

// Affiche le Render à l'écran
void WindowManager::updateScreen() const{
    SDL_RenderPresent(m_renderer);
}

// Permet de gérér les différents événements liées à la SDL :
// les évenements clavier/souris/quit
void WindowManager::eventLoop(){
    SDL_Event event;
    while(!m_quit){
        while(SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_MOUSEBUTTONUP:
                    onMouseClick(event.button.x, event.button.y, event.button.button);
                    break;
                case SDL_MOUSEWHEEL:
                    onMouseWheel(event.wheel.y);
                    break;
                case SDL_MOUSEMOTION:
                    m_mousePosition.x = event.motion.x;
                    m_mousePosition.y = event.motion.y;
                    onMouseMove(event.motion.x, event.motion.y);
                    break;
                case SDL_KEYDOWN:
                    onKeyboardDown(event.key.keysym.sym);
                    break;
                case SDL_WINDOWEVENT:
                    switch(event.window.event){
                        case SDL_WINDOWEVENT_RESIZED:
                            onResized(event.window.data1, event.window.data2);
                            break;
                        case SDL_WINDOWEVENT_MOVED:
                            blitTexture();
                            updateScreen();
                            break;
                    }
                    break;
                case SDL_QUIT:
                    m_quit = true;
                    break;
            }
        }
        SDL_Delay(10);
    }
}

// Getter qui retourne la position de la texture
const SDL_Rect& WindowManager::getTexturePosition() const{
    return m_texturePosition;
}

// Setter qui permet de modifier la position de la texture
void WindowManager::setTexturePosition(SDL_Rect texturePosition){
    m_texturePosition = texturePosition;
}

// Destructeur de la classe WindowManager
WindowManager::~WindowManager(){
    SDL_DestroyTexture(m_texture);
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

// Getter qui retourne la position de la souris
const SDL_Rect& WindowManager::getMousePosition() const{
    return m_mousePosition;
}

void WindowManager::screenShot() const{
    blitTexture();
    long currentTime = getCurrentTime();
    std::string fileName = std::to_string(currentTime);
    fileName = "screenshot/" + fileName + ".bmp";
    SDL_Surface* screenShot = SDL_CreateRGBSurface(0, m_texturePosition.w, m_texturePosition.h, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
    SDL_RenderReadPixels(m_renderer, &m_texturePosition, screenShot->format->format, screenShot->pixels, screenShot->pitch);
    SDL_SaveBMP(screenShot, fileName.data());
    SDL_FreeSurface(screenShot);
}










