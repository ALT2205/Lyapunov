#ifndef LYAPUNOV_LYAPUNOV_H
#define LYAPUNOV_LYAPUNOV_H

#include <SDL2/SDL_quit.h>
#include <vector>
#include <array>
#include "WindowManager.h"

#define NUMOFITER 100

#define X0 0.5

class Lyapunov : WindowManager {

    std::vector<Uint32> m_pixels;
    std::string m_sequence;
    SDL_Rect m_size;
    float m_aStart{0}, m_bStart{0}, m_aEnd{4}, m_bEnd{4};

    void generateSequence();

public:

    Lyapunov(unsigned int windowWidth, unsigned int windowHeight, unsigned int lyapunovWidth,
             unsigned int lyapunovHeight);

    void generate(float aStart = 0, float bStart = 0, float aEnd = 4, float bEnd = 4);

    std::array<float, 2> getCoordinates(int x, int y);

    void setPixelRGB(unsigned int index, unsigned int r, unsigned int g, unsigned int b);

    void setPixelHSV(unsigned int index, float h, float s, float v);

    void updatePixels();

    void onResized(unsigned int newWidth, unsigned int newHeight) override;

    void onMouseClick(unsigned int x, unsigned int y) override;

    void onMouseMove(unsigned int x, unsigned int y) override;

    void startLoop();
};

#endif //LYAPUNOV_LYAPUNOV_H