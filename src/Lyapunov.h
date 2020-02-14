#ifndef LYAPUNOV_LYAPUNOV_H
#define LYAPUNOV_LYAPUNOV_H

#include <SDL2/SDL_quit.h>
#include <vector>
#include "WindowManager.h"

#define NUMOFITER 700

#define X0 0.5

class Lyapunov : WindowManager{

    std::vector<Uint32> m_pixels;
    std::string m_sequence;
    SDL_Rect m_size;

    void generateSequence();

public:

    Lyapunov(unsigned int windowWidth, unsigned int windowHeight, unsigned int lyapunovWidth,
             unsigned int lyapunovHeight);

    void setPixelRGB(unsigned int index, unsigned int r, unsigned int g, unsigned int b);
    void setPixelHSV(unsigned int index, float h, float s, float v);
    void updatePixels();
    void startLoop();
    void onResized(unsigned int newWidth, unsigned int newHeight) override;
    void generate(float aStart = 0, float bStart = 0, float aEnd = 4, float bEnd = 4);

};

#endif //LYAPUNOV_LYAPUNOV_H