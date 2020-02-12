#include <iostream>
#include <cmath>
#include <cstdlib>
#include "Lyapunov.h"
#include <string>

/* Couleur : Jaune si Exposant < 0
   Couleur : Bleu si Exposant > 0
   https://en.wikipedia.org/wiki/Lyapunov_fractal
   Calcul de l'xposant de Lyapunov via https://www.youtube.com/watch?v=8xZyA09zRXY
   Quel logarithme doit-on utiliser ? LN ? LOG10 ? LOGN ? LOG2 ?
   Quel bibliothèque graphique ? Une qui permet de modif pixels par pixels ? QT? OpenGL/SDL ? GTK+ */

// Degradé au format brut : Pourquoi ne pas passer par un fichier ?

void Lyapunov::setPixel(unsigned int index, unsigned int r, unsigned int g, unsigned int b){
    m_pixels[index] = (r << 16) + (g << 8) + b;
}

void Lyapunov::updatePixels(){
    m_windowManager.update(m_pixels);
}

Lyapunov::Lyapunov() : m_windowManager(HEIGHT, WIDTH), m_pixels(HEIGHT * WIDTH){

}

void Lyapunov::eventLoop(){
    m_windowManager.eventLoop();
}

void Lyapunov::generateSequence(){
    std::cout << "Entrez la sequence de A-B\n";
    std::string sequence;
    //std::cin >> seq;
    sequence = "BA";
    while(m_sequence.length() < NUMOFITER){
        m_sequence += sequence;
    }
}

void Lyapunov::generate(){
    if(m_sequence.empty()){
        generateSequence();
    }
    std::vector<Uint32> pixels(WIDTH * HEIGHT);
    int greenLayer, redLayer, blueLayer;
    unsigned int i, x, y, yPos, index;
    double a, b, arrayExpo, expoLyap, xn, rn;
    // Echelle d'espacement entre chaque a/b pour x/y
    double scaleOfA = ((BORNESUPA - BORNEINFA) / WIDTH); //+ BORNEINFA;
    //Formule à corriger ?
    double scaleOfB = ((BORNESUPB - BORNEINFB) / HEIGHT); //+ BORNEINFB;
    //std::cout << scaleOfA << std::endl;
    //std::cout << scaleOfB << std::endl;
    for(y = 0; y < HEIGHT; ++y){
        std::cout << (float) y / WIDTH * 100 << "%" << std::endl;
        yPos = y * WIDTH;
        for(x = 0; x < WIDTH; ++x){
            index = yPos + x;
            // Calcul la position de X/Y dnas A/B
            a = x * scaleOfA;
            b = y * scaleOfB;
            expoLyap = 0;
            xn = X0;
            for(i = 0; i < NUMOFITER; ++i){
                // Choix entre A ou B selon le current Char
                rn = m_sequence[i] == 'A' ? a : b;
                // Calcul de Xn+1
                xn = rn * xn * (1 - xn);
                expoLyap += log2(fabs(rn * (1 - 2 * xn)));
            }
            arrayExpo = expoLyap /= NUMOFITER;
            //LAMBDA NEGATIF
            greenLayer = ((int) (210 + expoLyap * 50) >= 0) ? (int) (210 + expoLyap * 50) : 0;
            redLayer = ((int) (255 + expoLyap * 52) >= 100) ? (int) (255 + expoLyap * 52) : 100;
            blueLayer = ((int) (255 - expoLyap * 200) >= 0) ? (int) (255 - expoLyap * 200) : 0;
            //std::cout << greenLayer << std::endl;
            if(arrayExpo < -6){
                setPixel(index, 0, 0, 0);
            } else if(arrayExpo <= 0){
                setPixel(index, 0, greenLayer, 0);
            } else if(arrayExpo > 0){
                setPixel(index, 0, 0, blueLayer);
            } else if(arrayExpo >= 1){
                setPixel(index, 0, 0, 0);
            }
        }
    }
    updatePixels();
    std::cout << "Generation finie\n";
}

int main(){
    Lyapunov lyapunov;
    lyapunov.generate();
    lyapunov.eventLoop();
    return EXIT_SUCCESS;
}
