#include <iostream>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <SDL2/SDL.h>
#include <fstream>

#define WIDTH 600
#define HEIGHT 600

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

double scaleA() { // Echelle d'espacement entre chaque a/b pour x/y
  return ((BORNESUPA-BORNEINFA)/WIDTH); //+ BORNEINFA;
}
// FORMULE A CORRIGER
double scaleB() {
  return ((BORNESUPB-BORNEINFB)/HEIGHT); //+ BORNEINFB;
}

std::string AB(std::string seq){ // Complete la string de la taille de la FENETRE
  std::string toReturn;
  while (toReturn.length() < NUMOFITER){
    toReturn+=seq;
  }
  return toReturn;
}

double aOrB(double scale, double j){ // j désigne soit A soit B
  return j*scale; // Calcul la position de X/Y dnas A/B
}

double nextXN(double xn, double j) { // Calcul de Xn+1
  return j*xn*(1-xn);
}

double rN(char currChar,double a,double b){ // Choix entre A ou B selon le current Char
  return ((currChar=='A')?a:b);
}

double **exposant(std::string seq){
  double ** arrayExpo = new double* [WIDTH]; // Creation d'un tableau en 2D afin de recuperer un tableau contenant les exposant
  for (int i=0; i < WIDTH; i++)
    arrayExpo[i] = new double[HEIGHT];
  double a,b;
  static double scaleOfA = scaleA();
  std::cout << scaleOfA << std::endl;
  static double scaleOfB = scaleB();
  std::cout << scaleOfB << std::endl;
  double expoLyap;
  for(int x = 0;x<WIDTH;++x){
    for(int y = 0;y<HEIGHT;++y){
      a=aOrB(scaleOfA,x);
      b=aOrB(scaleOfB,y);
      expoLyap=0;

      double xn = X0;
      double rn = 0;

      for(long unsigned int i=0; i<NUMOFITER;++i){
	        rn = rN(seq[i],a,b);
          xn = nextXN(xn,rn);
	        expoLyap += log2(fabs(rn*(1-2*xn))) ;
      }
      expoLyap=expoLyap / NUMOFITER;
      arrayExpo[x][y]=expoLyap;
    }
  }
  return arrayExpo;
}


int main(){ // Revoir l'implémentation graphique de Lyapu mais semble fonctionnelle au vue de la tete du tableau en 10 10 sur AAB;
            // Implementation du code en PNM PORTABLE PIXMAP A FAIRE : APPRENDRE ECRITURE FICHIER + LECTURE
  std::string seq = "AAB";
  seq=AB(seq);
  std::ofstream data("data.ppm"); // Création du fichier ou est stockée l'image
  data << "P3";;
  data << std::endl;
  data << "600 600";
  data << std::endl;
  data << "255";
  data << std::endl;

  double ** arrayOfLyapu=exposant(seq);
  for(int i=0;i<WIDTH;++i){
    for(int j=0;j<HEIGHT;++j){
      //std::cout << arrayOfLyapu[i][j];
      if(arrayOfLyapu[i][j] < -2){
        data << " 184 134 11 ";
      }
      else if(arrayOfLyapu[i][j] >= -2 && arrayOfLyapu[i][j] < -1){
        data << " 255 215 0 ";
      }

      else if(arrayOfLyapu[i][j] >= -1 && arrayOfLyapu[i][j] < -0.5){
        data << " 255 228 54 ";
      }
      else if(arrayOfLyapu[i][j] >=-0.5 && arrayOfLyapu[i][j] <0 ){
        data << " 255 255 0";
      }
      else {
        data << " 0 0 255";
      }
    }
    //std::cout << std::endl;
    data << std::endl;
  }
  free(arrayOfLyapu);
  data.close();
  return EXIT_SUCCESS;
}
