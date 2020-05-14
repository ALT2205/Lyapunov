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
    Region m_currentRegion{0, 4, 0, 4};  // Correspond à la région d'affichage de la fractale
    int m_precision{100};               // Longueur de la séquence nécessaire au calcul de l'exposant
    double maxExpo{}, minExpo{};            // variables pour étirer les couleurs de la texture
    long m_lastMove{getCurrentTime()};  // Temps depuis
    std::stack<Region> m_lastPosition;  // Pile qui contient les régions zoomées
    Uint32 m_colorLyap[4]{};              // bornes de couleurs dans le même ordre que sur le menu
    int colorScale[12]{};                 /* Tableau contenant les variations de couleurs et
                                           les couleurs sur lesquelles l'appliquer */
    int m_zoomPrecision = 400;

    void generateSequence();

public:

    Lyapunov(unsigned int lyapunovWidth, unsigned int lyapunovHeight);

    void generate(Region region = {0, 4, 0, 4});

    void generatePart(int xStart, int yStart, int xEnd, int yEnd);

    Region getRegion(int fromX, int toX, int fromY, int toY);

    void setColorScale(int tab, int max, int min);

    void drawZoom();

    void validateRegion(int& x, int& y, int& w, int& h);

    void updatePixels();

    void updateSettings();

    void onResized(unsigned int newWidth, unsigned int newHeight) override;

    void onMouseClick(int mouseX, int mouseY, int button) override;

    void onMouseMove() override;

    void onMouseWheel(int amount) override;

    void onKeyboardDown(int c) override;

    void startLoop();
};

#endif //LYAPUNOV_LYAPUNOV_H
