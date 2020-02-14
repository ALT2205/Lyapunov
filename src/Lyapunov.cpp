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
   Quel bibliothèque graphique ? Une qui permet de modif pixels par pixels ? QT? OpenGL/SDL ? GTK+
    --> SDL

    Degradé au format brut : Pourquoi ne pas passer par un fichier ?*/

Lyapunov::Lyapunov(unsigned int windowWidth, unsigned int windowHeight,
                   unsigned int lyapunovWidth, unsigned int lyapunovHeight)
        : WindowManager(windowWidth, windowHeight), m_pixels(lyapunovWidth * lyapunovHeight), m_size(){
    m_size.w = (int) lyapunovWidth;
    m_size.h = (int) lyapunovHeight;
    SDL_Rect texturePosition;
    //La texture ne peut pas sortir de l'écran
    texturePosition.w = (int)(windowWidth < lyapunovWidth ? windowWidth : lyapunovWidth);
    texturePosition.h = (int)(windowHeight < lyapunovHeight ? windowHeight : lyapunovHeight);
    //La texture est carrée
    texturePosition.w = texturePosition.h = texturePosition.w < texturePosition.h ? texturePosition.w : texturePosition.h;
    //La texture est positionnée au milieu de l'écran
    texturePosition.x = (int) ((windowWidth >> 1u) - ((unsigned int)texturePosition.w >> 1u));
    texturePosition.y = (int) ((windowHeight >> 1u) - ((unsigned int)texturePosition.h >> 1u));
    initRender(m_size, texturePosition);
}

void Lyapunov::setPixelRGB(unsigned int index, unsigned int r, unsigned int g, unsigned int b){
    m_pixels[index] = (r << 16u) + (g << 8u) + b;
}

/*
 * Algorithme de conversion: https://fr.wikipedia.org/wiki/Teinte_Saturation_Valeur#Conversion_de_TSV_vers_RVB
 * Faut-il déclarer des variables ou bien passer directement en paramètre au détriment de la lisibilité ?
 * Y a-t-il un impact sur les performances en déclarant si appelé beaucoup de fois en peu de temps ?
 */
void Lyapunov::setPixelHSV(unsigned int index, float h, float s, float v){
    int hi = (int) (h / 60) % 6;
    switch(hi){
        case 0:
            setPixelRGB(index,
                        (int) (v * 255), //v
                        (int) (v * (1 - (1 - (h / 60 - (float)hi)) * s) * 255), //n
                        (int) (v * (1 - s) * 255)); //l
            break;
        case 1:
            setPixelRGB(index,
                        (int) (v * (1 - (h / 60 - (float)hi) * s) * 255), //m
                        (int) (v * 255), //v
                        (int) (v * (1 - s) * 255)); //l
            break;
        case 2:
            setPixelRGB(index,
                        (int) (v * (1 - s) * 255), //l
                        (int) (v * 255), //v
                        (int) (v * (1 - (1 - (h / 60 - (float)hi)) * s) * 255)); //n
            break;
        case 3:
            setPixelRGB(index,
                        (int) (v * (1 - s) * 255), //l
                        (int) (v * (1 - (h / 60 - (float)hi) * s) * 255), //m
                        (int) (v * 255)); //v
            break;
        case 4:
            setPixelRGB(index,
                        (int) (v * (1 - (1 - (h / 60 - (float)hi)) * s) * 255), //n
                        (int) (v * (1 - s) * 255), //l
                        (int) (v * 255)); //v
            break;
        case 5:
            setPixelRGB(index,
                        (int) (v * 255), //v
                        (int) (v * (1 - s) * 255), //l
                        (int) (v * (1 - (h / 60 - (float)hi) * s) * 255)); //m
            break;
        default:
            break;
    }
}

void Lyapunov::updatePixels(){
    update(m_pixels);
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
    std::vector<Uint32> pixels(m_size.w * m_size.h);
    int greenLayer, redLayer, blueLayer;
    unsigned int i, x, y, yPos, index;
    double a, b, expoLyap, xn, rn;
    // Echelle d'espacement entre chaque a/b pour x/y
    double scaleOfA = ((BORNESUPA - BORNEINFA) / m_size.w); //+ BORNEINFA;
    //Formule à corriger ?
    double scaleOfB = ((BORNESUPB - BORNEINFB) / m_size.h); //+ BORNEINFB;
    //std::cout << scaleOfA << std::endl;
    //std::cout << scaleOfB << std::endl;
    for(y = 0; y < m_size.h; ++y){
        std::cout << y * 100 / m_size.w  << "%" << std::endl;
        yPos = y * m_size.w;
        for(x = 0; x < m_size.w; ++x){
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
            expoLyap /= NUMOFITER;
            //LAMBDA NEGATIF
            greenLayer = ((int) (210 + expoLyap * 50) >= 0) ? (int) (210 + expoLyap * 50) : 0;
            redLayer = ((int) (255 + expoLyap * 52) >= 100) ? (int) (255 + expoLyap * 52) : 100;
            blueLayer = ((int) (255 - expoLyap * 200) >= 0) ? (int) (255 - expoLyap * 200) : 0;
            //std::cout << greenLayer << std::endl;
            if(expoLyap < -6){
                setPixelRGB(index, 0, 0, 0);
            } else if(expoLyap <= 0){
                setPixelRGB(index, 0, greenLayer, 0);
            } else if(expoLyap > 0){
                setPixelRGB(index, 0, 0, blueLayer);
            } else if(expoLyap >= 1){
                setPixelRGB(index, 0, 0, 0);
            }
        }
    }
    updatePixels();
    std::cout << "Generation finie\n";
}

void Lyapunov::onResized(unsigned int newWidth, unsigned int newHeight){
    SDL_Rect newPos{getTexturePosition()};
    newPos.x = (int) ((newWidth >> 1u) - ((unsigned int)newPos.w >> 1u));
    newPos.y = (int) ((newHeight >> 1u) - ((unsigned int)newPos.h >> 1u));
    newPos.w = newPos.h = (int)(newWidth < newHeight ? newWidth : newHeight);
    setTexturePosition(newPos);
    showTexture();
}

void Lyapunov::startLoop(){
    eventLoop();
}

int main(){
    Lyapunov lyapunov(200, 200, 1000, 1000);
    lyapunov.generate();
    lyapunov.startLoop();
    return EXIT_SUCCESS;
}
