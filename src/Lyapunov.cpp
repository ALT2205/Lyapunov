#include <iostream>
#include <cmath>
#include <cstdlib>
#include "Lyapunov.h"
#include <string>
#include <array>
#include <ctime>
#include <thread>


/* Couleur : Jaune si Exposant < 0
   Couleur : Bleu si Exposant > 0
   https://en.wikipedia.org/wiki/Lyapunov_fractal
   Calcul de l'xposant de Lyapunov via https://www.youtube.com/watch?v=8xZyA09zRXY
   Quel logarithme doit-on utiliser ? LN ? LOG10 ? LOGN ? LOG2 ?
   Quel bibliothèque graphique ? Une qui permet de modif pixels par pixels ? QT? OpenGL/SDL ? GTK+
    --> SDL

    Degradé au format brut : Pourquoi ne pas passer par un fichier ?*/

Lyapunov::Lyapunov(unsigned int windowWidth, unsigned int windowHeight,
                   unsigned int lyapunovWidth, unsigned int lyapunovHeight)
        : WindowManager(windowWidth, windowHeight), m_pixels(lyapunovWidth * lyapunovHeight), m_size() {
    m_size.w = (int) lyapunovWidth;
    m_size.h = (int) lyapunovHeight;
    SDL_Rect texturePosition;
    //La texture fait la taille de l'écran
    texturePosition.w = (int) windowWidth;
    texturePosition.h = (int) windowHeight;
    //Mais la texture est carrée
    texturePosition.w = texturePosition.h =
            texturePosition.w < texturePosition.h ? texturePosition.w : texturePosition.h;
    //La texture est positionnée au milieu de l'écran
    texturePosition.x = (int) ((windowWidth >> 1u) - ((unsigned int) texturePosition.w >> 1u));
    texturePosition.y = (int) ((windowHeight >> 1u) - ((unsigned int) texturePosition.h >> 1u));
    initRender(m_size, texturePosition);
}

std::array<float, 2> Lyapunov::getCoordinates(int x, int y) {
    std::array<float, 2> coordinates{};
    SDL_Rect texturePosition = getTexturePosition();
    //Les coordonnées ne peuvent pas sortir de la texture
    coordinates[0] = (float) (x < texturePosition.x ? 0 : x > texturePosition.x + texturePosition.w ?
                                                          texturePosition.w : x - texturePosition.x) /
                     (float) texturePosition.w * (m_aEnd - m_aStart) + m_aStart;
    coordinates[1] = (float) (y < texturePosition.y ? 0 : y > texturePosition.y + texturePosition.h ?
                                                          texturePosition.h : y - texturePosition.y) /
                     (float) texturePosition.h * (m_bEnd - m_bStart) + m_bStart;
    return coordinates;
}

void Lyapunov::setPixelRGB(unsigned int index, unsigned int r, unsigned int g, unsigned int b) {
    m_pixels[index] = (r << 16u) + (g << 8u) + b;
}

/*
 * Algorithme de conversion: https://fr.wikipedia.org/wiki/Teinte_Saturation_Valeur#Conversion_de_TSV_vers_RVB
 * Faut-il déclarer des variables ou bien passer directement en paramètre au détriment de la lisibilité ?
 * Y a-t-il un impact sur les performances en déclarant si appelé beaucoup de fois en peu de temps ?
 */
void Lyapunov::setPixelHSV(unsigned int index, float h, float s, float v) {
    int hi = (int) (h / 60) % 6;
    switch (hi) {
        case 0:
            setPixelRGB(index,
                        (int) (v * 255), //v
                        (int) (v * (1 - (1 - (h / 60 - (float) hi)) * s) * 255), //n
                        (int) (v * (1 - s) * 255)); //l
            break;
        case 1:
            setPixelRGB(index,
                        (int) (v * (1 - (h / 60 - (float) hi) * s) * 255), //m
                        (int) (v * 255), //v
                        (int) (v * (1 - s) * 255)); //l
            break;
        case 2:
            setPixelRGB(index,
                        (int) (v * (1 - s) * 255), //l
                        (int) (v * 255), //v
                        (int) (v * (1 - (1 - (h / 60 - (float) hi)) * s) * 255)); //n
            break;
        case 3:
            setPixelRGB(index,
                        (int) (v * (1 - s) * 255), //l
                        (int) (v * (1 - (h / 60 - (float) hi) * s) * 255), //m
                        (int) (v * 255)); //v
            break;
        case 4:
            setPixelRGB(index,
                        (int) (v * (1 - (1 - (h / 60 - (float) hi)) * s) * 255), //n
                        (int) (v * (1 - s) * 255), //l
                        (int) (v * 255)); //v
            break;
        case 5:
            setPixelRGB(index,
                        (int) (v * 255), //v
                        (int) (v * (1 - s) * 255), //l
                        (int) (v * (1 - (h / 60 - (float) hi) * s) * 255)); //m
            break;
        default:
            break;
    }
}

void Lyapunov::updatePixels() {
    updateTexture(m_pixels);
}

void Lyapunov::generateSequence() {
    //std::cout << "Entrez la sequence de A-B\n";
    std::string sequence;
    //std::cin >> seq;
    sequence = "BAAB";
    while (m_sequence.length() < NUMOFITER) {
        m_sequence += sequence;
    }
}

void
Lyapunov::generate(float aStart, float bStart, float aEnd, float bEnd) {

    if (aStart < 0 || aEnd > 4 || bStart < 0 || bEnd > 4) {
        throw std::domain_error("Invalid domain to generate Lyapunov");
    }
    m_aStart = aStart;
    m_aEnd = aEnd;
    m_bStart = bStart;
    m_bEnd = bEnd;
    if (m_sequence.empty()) {
        generateSequence();
    }
    if (aStart > aEnd) {
        float change = aStart;
        aStart = aEnd;
        aEnd = change;
    }
    if (bStart > bEnd) {
        float change = bStart;
        bStart = bEnd;
        bEnd = change;
    }

    // Creation du nombre de threads en fonction du nombre de threads de l'ordinateur
    int nbThread = std::thread::hardware_concurrency();
    std::vector<std::thread> threads(nbThread);
    for (int i = 0; i < nbThread; i++) {
        threads[i] = std::thread(&Lyapunov::generatePart,this,(unsigned int)0,(unsigned int)i*m_size.w/nbThread,(unsigned int) m_size.w, (unsigned int)(i+1)*m_size.h/nbThread);
    }
    for (auto& th : threads) {
        th.join();
    }
    updatePixels();
    blitTexture();
    updateScreen();
    //std::cout << "Generation finie\n";
}




void
Lyapunov::generatePart(unsigned int xStart, unsigned int yStart, unsigned int xEnd, unsigned int yEnd){

    //Utilisation de variable local pour améliorer la performance
    unsigned int width = m_size.w, height = m_size.h;
    int greenLayer, redLayer, blueLayer;
    unsigned int i, x, y, yPos, index;
    double a, b, expoLyap, xn, rn;
    // Echelle d'espacement entre chaque a/b pour x/y
    double scaleOfA = ((m_aEnd - m_aStart) / (float) width);
    double scaleOfB = ((m_bEnd - m_bStart) / (float) height);
    for (y = yStart; y < yEnd; ++y) {
        //std::cout << y * 100 / width << "%" << std::endl;
        yPos = y * width;
        for (x = xStart; x < xEnd; ++x) {
            index = yPos + x;
            // Calcul la position de X/Y dans A/B
            a = m_aStart + x * scaleOfA;
            b = m_bStart + y * scaleOfB;
            expoLyap = 0;
            xn = X0;
            for (i = 0; i < NUMOFITER; ++i) {

                // Choix entre A ou B selon le current Char
                rn = m_sequence[i] == 'A' ? a : b;

                // Calcul de Xn+1
                xn = rn * xn * (1 - xn);

                expoLyap += log2(fabs(rn * (1 - 2 * xn)));
            }
            expoLyap /= NUMOFITER;
            //LAMBDA NEGATIF
            greenLayer = ((int) (210 + expoLyap * 50) >= 0) ? (int) (210 + expoLyap * 50) : 0;
            redLayer = ((int) (255 + expoLyap * 52) >= 100) ? (int) (255 + expoLyap * 52) : 100;
            blueLayer = ((int) (255 - expoLyap * 200) >= 0) ? (int) (255 - expoLyap * 200) : 0;
            if (expoLyap < -6) {
                setPixelRGB(index, 0, 0, 0);
            } else if (expoLyap <= 0) {
                setPixelRGB(index, 0, greenLayer, 0);
            } else if (expoLyap > 0) {
                setPixelRGB(index, 0, 0, blueLayer);
            } else if (expoLyap >= 1) {
                setPixelRGB(index, 0, 0, 0);
            }
        }
    }
}




void Lyapunov::onResized(unsigned int newWidth, unsigned int newHeight) {
    SDL_Rect newPos;
    newPos.w = newPos.h = (int) (newWidth < newHeight ? newWidth : newHeight);
    newPos.x = (int) ((newWidth >> 1u) - ((unsigned int) newPos.w >> 1u));
    newPos.y = (int) ((newHeight >> 1u) - ((unsigned int) newPos.h >> 1u));
    setTexturePosition(newPos);
    updateScreen();
}

void Lyapunov::onMouseClick(unsigned int x, unsigned int y) {
    std::array<float, 2> coordsStart = getCoordinates((int) x - 200, (int) y - 200);
    std::array<float, 2> coordsEnd = getCoordinates((int) x + 200, (int) y + 200);
    generate(coordsStart[0], coordsStart[1], coordsEnd[0], coordsEnd[1]);
}

void Lyapunov::onMouseMove(unsigned int x, unsigned int y) {
    std::array<float, 2> coords = getCoordinates((int) x, (int) y);
    blitTexture();
    drawRect((int) x - 200, (int) y - 200, 400, 400);
    updateScreen();
}

void Lyapunov::startLoop() {
    eventLoop();
}

int main() {

    Lyapunov lyapunov(1400, 1000, 1000, 1000);
    lyapunov.generate(0, 0, 4.0, 4.0);
    lyapunov.startLoop();
    return EXIT_SUCCESS;
}
