#ifndef LYAPUNOV_LYAPUNOV_H
#define LYAPUNOV_LYAPUNOV_H

#include <SDL2/SDL_quit.h>
#include <vector>
#include <array>
#include <string>
#include <thread>
#include <cmath>
#include <cstdlib>
#include <stack>
#include "WindowManager.h"
#include "Time.h"
#include "Region.h"
#include "Menu.h"

#define X0 0.5


// Classe principale de la génération de fractales de Lyapunov.

class Lyapunov : WindowManager {

    std::vector<double> m_exponents;    // le tableau 2D des exposants de Lyapunov
    std::string m_sequence;             // sequence de A et B pour le calcul des exposants
    SDL_Rect m_size;                    // Taille de la fractale de Lyapunov
    Region m_curentRegion{0, 4, 0, 4};  // Correspond à la région d'affichage de la fractale
    int m_precision{100};               // Longueur de la séquence nécessaire au calcul de l'exposant
    long m_lastMove{getCurrentTime()};  // Temps depuis
    int m_currentColor{0};              //
    bool m_stopColor{false};            // Determine si les couleurs de la fractale changent
    std::stack<Region> m_lastPosition;  // Pile qui contient les régions zoomées

    void generateSequence();

public:

    Lyapunov(unsigned int windowWidth, unsigned int windowHeight, unsigned int lyapunovWidth,
             unsigned int lyapunovHeight);

    void generate(Region region = {0, 4, 0, 4});

    void generatePart(unsigned int xStart, unsigned int yStart, unsigned int xEnd, unsigned int yEnd);

    Region getRegion(int fromX, int toX, int fromY, int toY);

    void setPixelRGB(std::vector<Uint32>& pixels, unsigned int index, unsigned int r, unsigned int g, unsigned int b);

    void setPixelHSV(std::vector<Uint32>& pixels, unsigned int index, int h, double s, double v);

    void updatePixels();

    void onResized(unsigned int newWidth, unsigned int newHeight) override;

    void onMouseClick(unsigned int x, unsigned int y, unsigned int button) override;

    void onMouseMove(unsigned int x, unsigned int y) override;

    void onMouseWheel() override;

    void onKeyboard(int c) override;

    void onTick() override;

    void startLoop();
};

#endif //LYAPUNOV_LYAPUNOV_H
