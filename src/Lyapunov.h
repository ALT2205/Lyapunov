#ifndef LYAPUNOV_LYAPUNOV_H
#define LYAPUNOV_LYAPUNOV_H

#include <SDL2/SDL_quit.h>
#include <vector>
#include "WindowManager.h"

#define WIDTH 700
#define HEIGHT 700

#define BORNEINFA 0.0
#define BORNESUPA 4.0

#define BORNEINFB 0.0
#define BORNESUPB 4.0

#define NUMOFITER 700

#define X0 0.5

class Lyapunov {

    WindowManager windowManager;
    std::vector<Uint32> pixels;

public:

    Lyapunov();

    void setPixel(int index, unsigned int r, unsigned int g, unsigned int b);
    void setPixels(std::vector<Uint32> pixels);
    void eventLoop();

};

#endif //LYAPUNOV_LYAPUNOV_H