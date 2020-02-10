#include <iostream>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include "WindowManager.h"
#include <fstream>
#include <string>


#define WIDTH 700
#define HEIGHT 700

#define BORNEINFA 0.0
#define BORNESUPA 4.0

#define BORNEINFB 0.0
#define BORNESUPB 4.0

#define NUMOFITER 700

#define X0 0.5

//Couleur : Jaune si Exposant < 0
//Couleur : Bleu si Exposant > 0
//https://en.wikipedia.org/wiki/Lyapunov_fractal
// Calcul de l'xposant de Lyapunov via https://www.youtube.com/watch?v=8xZyA09zRXY
// Quel logarithme doit-on utiliser ? LN ? LOG10 ? LOGN ? LOG2 ?
// Quel bibliothèque graphique ? Une qui permet de modif pixels par pixels ? QT? OpenGL/SDL ? GTK+

// Degradé au format brut : Pourquoi ne pas passer par un fichier ?

double scaleA(){ // Echelle d'espacement entre chaque a/b pour x/y
    return ((BORNESUPA - BORNEINFA) / WIDTH); //+ BORNEINFA;
}

// FORMULE A CORRIGER
double scaleB(){
    return ((BORNESUPB - BORNEINFB) / HEIGHT); //+ BORNEINFB;
}

std::string AB(std::string seq){ // Complete la string de la taille de la FENETRE
    std::string toReturn;
    while(toReturn.length() < NUMOFITER){
        toReturn += seq;
    }
    return toReturn;
}

double aOrB(double scale, double j){ // j désigne soit A soit B
    return j * scale; // Calcul la position de X/Y dnas A/B
}

double nextXN(double xn, double j){ // Calcul de Xn+1
    return j * xn * (1 - xn);
}

double rN(char currChar, double a, double b){ // Choix entre A ou B selon le current Char
    return ((currChar == 'A') ? a : b);
}

Uint32 colorToPixel(int red, int green, int blue){
    Uint32 pixel =+ (255 << 24) + (red << 16) + (green << 8) + (blue << 0);
    return pixel;
}


Uint32* exposant(std::string seq){
    Uint32* pixels = new Uint32[WIDTH * HEIGHT];
    std::ofstream data("data.ppm"); // Création du fichier ou est stockée l'image
    data << "P3";
    data << std::endl;
    data << std::to_string(WIDTH);
    data << " ";
    data << std::to_string(HEIGHT);
    data << std::endl;
    data << "255";
    data << std::endl;

    int greenLayer,redLayer, blueLayer;
    double** arrayExpo = new double* [WIDTH]; // Creation d'un tableau en 2D afin de recuperer un tableau contenant les exposant
    for(int i = 0; i < WIDTH; i++)
        arrayExpo[i] = new double[HEIGHT];
    double a, b;
    static double scaleOfA = scaleA();
    std::cout << scaleOfA << std::endl;
    static double scaleOfB = scaleB();
    std::cout << scaleOfB << std::endl;
    double expoLyap;
    for(int x = 0; x < WIDTH; ++x){
        std::cout << (float)x / WIDTH * 100 << "%" << std::endl;
        for(int y = 0; y < HEIGHT; ++y){

            int index = y * WIDTH + x;
            a = aOrB(scaleOfA, x);
            b = aOrB(scaleOfB, y);
            expoLyap = 0;

            double xn = X0;
            double rn = 0;

            for(long unsigned int i = 0; i < NUMOFITER; ++i){
                rn = rN(seq[i], a, b);
                xn = nextXN(xn, rn);
                expoLyap += log2(fabs(rn * (1 - 2 * xn)));
            }
            expoLyap = expoLyap / NUMOFITER;
            arrayExpo[x][y] = expoLyap;

            //LAMBDA NEGATIF
            greenLayer = ( (int)210+expoLyap* 50 >= 0 )?(int) 210+expoLyap*50: 0 ;
            redLayer = ( (int)255+expoLyap* 52>= 100 )?(int) 255+expoLyap*52 : 100 ;

            blueLayer = ( (int)255-expoLyap*200 >= 0)?(int) 255-expoLyap*200 : 0 ;
            //std::cout << greenLayer << std::endl;
            if(arrayExpo[x][y] < -6){
                pixels[index] = colorToPixel(0,0,0);
            }
            else if (arrayExpo[x][y] <= 0){
                pixels[index] = colorToPixel(0,greenLayer,0);
            }
            else if (arrayExpo[x][y] > 0){
                pixels[index] = colorToPixel(0,0,blueLayer);
            }
            else if (arrayExpo[x][y] >= 1){
                pixels[index] = colorToPixel(0,0,0);
            }
        }

            /*
            if(arrayExpo[x][y] < -4){
                //data << " 100 45 0 ";
                pixels[y * WIDTH + x] = (100 << 16) + (45 << 8) + (0 << 0) + (255 << 24);
            } else if(arrayExpo[x][y] >= -4 && arrayExpo[x][y] < -3.7){
                //data << " 112 62 0 ";
                pixels[y * WIDTH + x] = (112 << 16) + (62 << 8) + (0 << 0) + (255 << 24);
            } else if(arrayExpo[x][y] >= -3.7 && arrayExpo[x][y] < -3.4){
                //data << " 124 79 0 ";
                pixels[y * WIDTH + x] = (124 << 16) + (79 << 8) + (0 << 0) + (255 << 24);
            } else if(arrayExpo[x][y] >= -3.4 && arrayExpo[x][y] < -3.1){
                //data << "  136 96 0 ";
                pixels[y * WIDTH + x] = (136 << 16) + (96 << 8) + (0 << 0) + (255 << 24);
            } else if(arrayExpo[x][y] >= -3.1 && arrayExpo[x][y] < -2.9){
                //data << " 148 113 0 ";
                pixels[y * WIDTH + x] = (148 << 16) + (113 << 8) + (0 << 0) + (255 << 24);
            } else if(arrayExpo[x][y] >= -2.9 && arrayExpo[x][y] < -2.6){
                //data << " 160 130 0 ";
                pixels[y * WIDTH + x] = (160 << 16) + (130 << 8) + (0 << 0) + (255 << 24);
            } else if(arrayExpo[x][y] >= -2.6 && arrayExpo[x][y] < -2.3){
                //data << " 172 147 0 ";
                pixels[y * WIDTH + x] = (172 << 16) + (147 << 8) + (0 << 0) + (255 << 24);
            } else if(arrayExpo[x][y] >= -2.3 && arrayExpo[x][y] < -2){
                //data << " 184 164 0 ";
                pixels[y * WIDTH + x] = (184 << 16) + (164 << 8) + (0 << 0) + (255 << 24);
            } else if(arrayExpo[x][y] >= -2 && arrayExpo[x][y] < -1.7){
                //data << " 196 181 0 ";
                pixels[y * WIDTH + x] = (196 << 16) + (181 << 8) + (0 << 0) + (255 << 24);
            } else if(arrayExpo[x][y] >= -1.7 && arrayExpo[x][y] < -1.4){
                //data << " 208 198 0 ";
                pixels[y * WIDTH + x] = (208 << 16) + (198 << 8) + (0 << 0) + (255 << 24);
            } else if(arrayExpo[x][y] >= -1.4 && arrayExpo[x][y] < -1.1){
                //data << " 220 215 0 ";
                pixels[y * WIDTH + x] = (220 << 16) + (215 << 8) + (0 << 0) + (255 << 24);
            } else if(arrayExpo[x][y] >= -1.1 && arrayExpo[x][y] < -0.8){
                //data << " 232 232 0 ";
                pixels[y * WIDTH + x] = (232 << 16) + (232 << 8) + (0 << 0) + (255 << 24);
            } else if(arrayExpo[x][y] >= -0.5 && arrayExpo[x][y] < -0.2){
                //data << " 244 249 0 ";
                pixels[y * WIDTH + x] = (244 << 16) + (249 << 8) + (0 << 0) + (255 << 24);
            } else if(arrayExpo[x][y] >= -0.2 && arrayExpo[x][y] < 0){
                //data << " 255 255 0 ";
                pixels[y * WIDTH + x] = (255 << 16) + (255 << 8) + (0 << 0) + (255 << 24);
            }
                // LAMBDA POSITIF
            else {
                //data << " 0 0 139 ";
                pixels[y * WIDTH + x] = (0 << 16) + (0 << 8) + (139 << 0) + (255 << 24);

            }
        }
             */
        data << std::endl;
    }
    data.close();
    return pixels;
}

int
main(){ // Revoir l'implémentation graphique de Lyapu mais semble fonctionnelle au vue de la tete du tableau en 10 10 sur AAB;
    // Implementation du code en PNM PORTABLE PIXMAP A FAIRE : APPRENDRE ECRITURE FICHIER + LECTURE
    std::cout << "Entrez la sequence de A-B\n";
    std::string seq;
    //std::cin >> seq;
    seq="BA";
    seq = AB(seq);
    WindowManager manager(WIDTH, HEIGHT);
    Uint32* arrayExpo = exposant(seq);
    std::cout << "Generation finie\n";
    manager.setPixels(arrayExpo);
    manager.update();
    manager.eventLoop();
    return EXIT_SUCCESS;
}
