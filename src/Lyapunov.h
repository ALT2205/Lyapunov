#ifndef LYAPUNOV_LYAPUNOV_H
#define LYAPUNOV_LYAPUNOV_H

#include <SDL2/SDL_quit.h>
#include <vector>
#include <array>
#include <string>
#include <thread>
#include <cmath>
#include <cstdlib>
#include "WindowManager.h"
#include "Time.h"

#define X0 0.5

class Lyapunov : WindowManager {

    std::vector<double> m_exponents;
    std::string m_sequence;
    SDL_Rect m_size;
    double m_aStart{0}, m_bStart{0}, m_aEnd{4}, m_bEnd{4};
    int precision{100};
    long m_lastMove{getCurrentTime()};
    int currentColor{0};
    bool stopColor{false};

    void generateSequence();

public:

    Lyapunov(unsigned int windowWidth, unsigned int windowHeight, unsigned int lyapunovWidth,
             unsigned int lyapunovHeight);

    void generate(double aStart = 0, double bStart = 0, double aEnd = 4, double bEnd = 4);

    void generatePart(unsigned int xStart, unsigned int yStart, unsigned int xEnd, unsigned int yEnd);

    std::array<double, 2> getCoordinates(int x, int y);

    void setPixelRGB(std::vector<Uint32>& pixels, unsigned int index, unsigned int r, unsigned int g, unsigned int b);

    void setPixelHSV(std::vector<Uint32>& pixels, unsigned int index, int h, double s, double v);

    void updatePixels();

    void onResized(unsigned int newWidth, unsigned int newHeight) override;

    void onMouseClick(unsigned int x, unsigned int y) override;

    void onMouseMove(unsigned int x, unsigned int y) override;

    void onMouseWheel() override;

    void onKeyboard(int c) override;

    void onTick() override;

    void startLoop();
};

#endif //LYAPUNOV_LYAPUNOV_H