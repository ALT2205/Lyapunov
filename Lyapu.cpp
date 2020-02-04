#include <iostream>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <SDL2/SDL.h>

#define WINDOW 10
#define X0 0.5

//Couleur : Jaune si Exposant < 0
//Couleur : Bleu si Exposant > 0
//https://en.wikipedia.org/wiki/Lyapunov_fractal
// Calcul de l'xposant de Lyapunov via https://www.youtube.com/watch?v=8xZyA09zRXY
// Quel logarithme doit-on utiliser ? LN ? LOG10 ? LOGN ? LOG2 ?
// Quel bibliothèque graphique ? Une qui permet de modif pixels par pixels ? QT? OpenGL/SDL ? GTK+

double scale(double borneSup, double borneInf) { // Echelle d'espacement entre chaque a/b pour x/y
  return (borneSup-borneInf)/WINDOW + borneInf;
}

std::string AB(std::string seq){ // Complete la string de la taille de la FENETRE
  std::string toReturn;
  while (toReturn.length() < WINDOW){
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

double **exposant(std::string seq, double scaleAB){
  double ** arrayExpo = new double* [WINDOW]; // Creation d'un tableau en 2D afin de recuperer un tableau contenant les exposant
  for (int i=0; i < WINDOW; i++)
    arrayExpo[i] = new double[WINDOW];
  double a,b;
  double expoLyap;
  for(int x = 0;x<WINDOW;++x){
    for(int y = 0;y<WINDOW;++y){
      a=aOrB(scaleAB,x);
      b=aOrB(scaleAB,y);
      expoLyap=0;

      double xn = X0;
      double rn = 0;

      for(long unsigned int i=0; i<seq.length();++i){
	xn = nextXN(xn,rn);
	rn = rN(seq[i%seq.length()],a,b);
	expoLyap += log(fabs(rn*(1-2*xn))/seq.length());
      }
      expoLyap /= seq.length();
      arrayExpo[x][y]=expoLyap;
    }
  }
  return arrayExpo;
}


int main(){ // Revoir l'implémentation graphique de Lyapu mais semble fonctionnelle au vue de la tete du tableau en 10 10 sur AAB;
  /*SDL_Event event;
  SDL_Renderer *renderer;
  SDL_Window *window;
  int i;

  SDL_Init(SDL_INIT_VIDEO);
  SDL_CreateWindowAndRenderer(WINDOW, WINDOW, 0, &window, &renderer);
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
  SDL_RenderClear(renderer);
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);*/
  std::string seq = "AAB";
  seq=AB(seq);
  double ** arrayOfLyapu=exposant(seq,scale(4,2.5));
  for(int i=0;i<WINDOW;++i){ // Soucis dans
    for(int j=0;j<WINDOW;++j){
      /*if (tab[i][j]<0){
	SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
      }
      else{
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
      }
      SDL_RenderDrawPoint(renderer, i, j);
    }
  }
  while (1) {
    if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
      break;
  }
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();*/
  std::cout << arrayOfLyapu[i][j] << " ";
    }
    std::cout << std::endl;
  }
  return EXIT_SUCCESS;
}
