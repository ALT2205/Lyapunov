#ifndef LYAPUNOV_LYAPUNOV_H
#define LYAPUNOV_LYAPUNOV_H

#include <SDL2/SDL_quit.h>
#include <vector>
#include "WindowManager.h"

#define BORNEINFA 0.0
#define BORNESUPA 4.0

#define BORNEINFB 0.0
#define BORNESUPB 4.0

#define NUMOFITER 700

#define X0 0.5

class Lyapunov : WindowManager{

    std::vector<Uint32> m_pixels;
    std::string m_sequence;

    void generateSequence();

public:

    Lyapunov(unsigned int width, unsigned int height, unsigned int lyapunovWidth,
             unsigned int lyapunovHeight);

    void setPixelRGB(unsigned int index, unsigned int r, unsigned int g, unsigned int b);
    void setPixelHSV(unsigned int index, float h, float s, float v);
    void updatePixels();
    void startLoop();
    void onResized(unsigned int newWidth, unsigned int newHeight) override;
    void generate();

};

#endif //LYAPUNOV_LYAPUNOV_H