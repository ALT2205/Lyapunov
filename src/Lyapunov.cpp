#include "Lyapunov.h"


// Constructeur de la classe Lyapunov
Lyapunov::Lyapunov(unsigned int windowWidth, unsigned int windowHeight,
                   unsigned int lyapunovWidth, unsigned int lyapunovHeight)
        : WindowManager(windowWidth, windowHeight), m_exponents(lyapunovWidth * lyapunovHeight), m_size(),
          m_lastPosition{}{



    // Ouverture en mode lecture du fichier de configuration
    std::ifstream file("config.txt");
    std::string str;
    int r,g,b,i = 0,precis;

    // On recupère d'abord les couleurs stockées dans un  tableau de Pixels(Uint32)
    while (i < 4 && file >> str >> r >> g >> b){
      m_colorLyap[i] = (r << 16u) + (g << 8u) + b;
      std::cout << r << g << b << std::endl;
      i++;
    }

    // Création des tableaux contenant les variations de couleurs en fonction des bornes
    setColorScale(0,m_colorLyap[1],m_colorLyap[0]); //Neg
    setColorScale(3,m_colorLyap[3],m_colorLyap[2]); //Pos



    // On recupère ensite la précision souhaitée
    file >> str >> precis;
    m_precision = precis;
    std::cout << m_precision << std::endl;


    // On récupère finalement la sequence
    std::string seq;
    file >> str >> seq;
    m_sequence = seq;
    std::cout << m_sequence << std::endl;
    generateSequence();
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


 // Renvoie la région sur le plan Lyapunov (entre 0 et 4)
 // à partir de la région de l'écran
Region Lyapunov::getRegion(int fromX, int toX, int fromY, int toY){
    SDL_Rect texturePosition = getTexturePosition();
    Region region{
            //Si x est négatif, alors 0, si x dépasse de la texture, alors la largeur de la texture
            //Ensuite on divise par la largeur de la texture, on multiplie par la largeur actuelle du plan de Lyapunov
            //Et on ajoute l'origine
            //TODO: Faire une fonction pour simplifier cette partie lourde à lire ?
            (double) (fromX < texturePosition.x ? 0 : fromX > texturePosition.x + texturePosition.w ? texturePosition.w : fromX - texturePosition.x) / (double) texturePosition.w * (m_curentRegion.getToX() - m_curentRegion.getFromX()) + m_curentRegion.getFromX(),
            (double) (toX < texturePosition.x ? 0 : toX > texturePosition.x + texturePosition.w ? texturePosition.w : toX - texturePosition.x) / (double) texturePosition.w * (m_curentRegion.getToX() - m_curentRegion.getFromX()) + m_curentRegion.getFromX(),
            (double) (fromY < texturePosition.y ? 0 : fromY > texturePosition.y + texturePosition.h ? texturePosition.h : fromY - texturePosition.y) / (double) texturePosition.h * (m_curentRegion.getToY() - m_curentRegion.getFromY()) + m_curentRegion.getFromY(),
            (double) (toY < texturePosition.y ? 0 : toY > texturePosition.y + texturePosition.h ? texturePosition.h : toY - texturePosition.y) / (double) texturePosition.h * (m_curentRegion.getToY() - m_curentRegion.getFromY()) + m_curentRegion.getFromY()};
    return region;
}

// Définie la couleur d'un pixel dans l'espace colorimétrique RGB
void Lyapunov::setPixelRGB(std::vector<Uint32>& pixels, unsigned int index,
                           unsigned int r, unsigned int g, unsigned int b){
    pixels[index] = (r << 16u) + (g << 8u) + b;
}



 // Définie la couleur d'un pixel dans l'espace colorimétrique HSV
 // Algorithme de conversion: https://fr.wikipedia.org/wiki/Teinte_Saturation_Valeur#Conversion_de_TSV_vers_RVB
 // Utilisée surtout pour faire le défilement des couleurs en mode arc-en-ciel

void Lyapunov::setPixelHSV(std::vector<Uint32>& pixels, unsigned int index, int h, double s, double v){
    h %= 360;
    int hi = (int) (h / 60) % 6;
    switch(hi){
        case 0:
            setPixelRGB(pixels, index,
                        (int) (v * 255), //v
                        (int) (v * (1 - (1 - (h / 60.0 - (double) hi)) * s) * 255), //n
                        (int) (v * (1 - s) * 255)); //l
            break;
        case 1:
            setPixelRGB(pixels, index,
                        (int) (v * (1 - (h / 60.0 - (double) hi) * s) * 255), //m
                        (int) (v * 255), //v
                        (int) (v * (1 - s) * 255)); //l
            break;
        case 2:
            setPixelRGB(pixels, index,
                        (int) (v * (1 - s) * 255), //l
                        (int) (v * 255), //v
                        (int) (v * (1 - (1 - (h / 60.0 - (double) hi)) * s) * 255)); //n
            break;
        case 3:
            setPixelRGB(pixels, index,
                        (int) (v * (1 - s) * 255), //l
                        (int) (v * (1 - (h / 60.0 - (double) hi) * s) * 255), //m
                        (int) (v * 255)); //v
            break;
        case 4:
            setPixelRGB(pixels, index,
                        (int) (v * (1 - (1 - (h / 60.0 - (double) hi)) * s) * 255), //n
                        (int) (v * (1 - s) * 255), //l
                        (int) (v * 255)); //v
            break;
        case 5:
            setPixelRGB(pixels, index,
                        (int) (v * 255), //v
                        (int) (v * (1 - s) * 255), //l
                        (int) (v * (1 - (h / 60.0 - (double) hi) * s) * 255)); //m
            break;
        default:
            break;
    }
}
void Lyapunov::setColorScale(int tab,Uint32 max,Uint32 min){

    int currMax = max;
    int currMin = min;
    for (int i = 2; i>0;i--){
        currMax = max%256;
        currMin = min%256;
        colorScale[tab + i] = currMax - currMin;
        max = (max-currMax) / 256;
        min = (max-currMax) / 256;
    } 

}

// Modifie la couleur des pixels  dans un tableau en
// fonction de la valeur de l'exposant associé à la même
// position dans le tableau des exposants 2D
void Lyapunov::updatePixels(){
    std::cout<<"in update Pixels"<<std::endl;
    std::vector<Uint32> pixels(m_size.w * m_size.h);
    for(int i = 0, size = m_size.w * m_size.h; i < size; ++i){
        double exponent = m_exponents[i];
        /*
        int green = ((int) (210 + exponent * 50) >= 0) ? (int) (210 + exponent * 50) :0 ;
        int red = ((int) (255 + exponent * 52) >= 100) ? (int) (255 + exponent * 52) : 100;
        int blue = ((int) (255 - exponent * 200) >= 0) ? (int) (255 - exponent * 200) : 0;
        */

        int red,green,blue;
        int choixTab =0;
        if(exponent > 0){
            choixTab =0;
        }

            red = (int) (colorScale[choixTab] * exponent) + m_colorLyap[1];
            green = (int) (colorScale[choixTab + 1] * exponent) + m_colorLyap[1];
            blue = (int) (colorScale[choixTab + 2] * exponent) + m_colorLyap[1];
            
        
        

        // Calcul des différentes couleurs d'après une série de tests.
        // en fonction de la valeur de l'exposant de lyapunov

        //int h = m_currentColor;
        //setPixelHSV(pixels, i, greenLayer + h, 1, 1);
        /*
        if(exponent < -6){
            setPixelRGB(pixels, i, 0, 0, 0);
        } else if(exponent <= 0){ //entre -1 et 0

            setPixelRGB(pixels, i, 0, green, 0);
        } else if(exponent > 0){//entre 0 et 1
            setPixelRGB(pixels, i, 0, 0, blue);
        } else if(exponent >= 1){  
            setPixelRGB(pixels, i, 0, 0, 0);
        }*/
 
        setPixelRGB(pixels, i, red, green, blue);
        
    }
    std::cout<<"before Texture"<<std::endl;
    updateTexture(pixels);
    std::cout<<"out update Pixels"<<std::endl;
}



// Génération de la séquence en fonction de la précision voulue
// et de la séquence initiale
void Lyapunov::generateSequence(){
    std::string sequence;
    bool error = false;
    std::cout << m_sequence.length()<< std::endl;
    for(unsigned int i = 0;i< m_sequence.length();i++){
      std::cout << m_sequence.at(i) << std::endl;
      if (m_sequence.at(i) != 'A' || m_sequence.at(i) != 'B'){
        std::cout << "An error in the construction of the sequence has been detected. Sequence must contains only A and B. Default Sequence : AB" << std::endl;
        error = true;
        break;
      }
    }
    if (m_sequence.empty() || error){
      m_sequence = "AB";
    }
    sequence = m_sequence;
    while((int)m_sequence.length() < m_precision){
        m_sequence += sequence;
    }
}


// Génère la fractale de Lyapunov dans une région donnée
void Lyapunov::generate(Region region){
    m_curentRegion = Region{region};

    if(region.getFromX() < 0 || region.getToX() > 4 || region.getFromY() < 0 || region.getToY() > 4){
        throw std::domain_error("Invalid domain to generate Lyapunov");
    }
    if(m_sequence.empty()){
        generateSequence();
    }


    // Cette partie de la focntion est responsable du multi-threading
    // nécessaire à la création des fractales de Lyapunov


    // Nombre de threads en fonction du nombre de threads du CPU
    // On garde un thread libre pour gérer d'eventuels problèmes
    unsigned int nbThread = std::thread::hardware_concurrency() ;

    // Vector de threads afin de pouvoir géré le multi-threading
    std::vector<std::thread> threads(nbThread) ;

    // Création des différents threads générant une partie de la fractale de Lyapunov
    for(unsigned int i = 0; i < nbThread; i++){
        threads[i] = std::thread(&Lyapunov::generatePart, this, 0,
                                 i * m_size.w / nbThread, m_size.w,
                                 (i + 1) * m_size.h / nbThread);
    }

    // Permet de synchroniser les différentes opérations gérées dans les threads.
    // Met également fin aux threads.
    for(auto& th : threads){
        th.join();
    }

    updatePixels();
    blitTexture();
    SDL_Rect mousePos = getMousePosition();
    // Tracer du rectangle affichant la zone Zoomée
    drawRect((int) mousePos.x - 200, (int) mousePos.y - 200, 400, 400);
    updateScreen();
}

// Permet de générer une partie de la fractale de Lyapunov
// Entre différentes coordonnées. Utile pour le multi-threading
// Calcul de l'exposant de Lyapunov : https://en.wikipedia.org/wiki/Lyapunov_fractal#Algorithm_for_generating_Lyapunov_fractals
void Lyapunov::generatePart(unsigned int xStart, unsigned int yStart, unsigned int xEnd, unsigned int yEnd){
    //Utilisation de variable local pour améliorer la performance
    unsigned int width = m_size.w, height = m_size.h,x, y, yPos, index,i,j;
    double a, b, expoLyap, xn, rn;
    // Echelle d'espacement entre chaque a/b pour x/y
    double aStart = m_curentRegion.getFromX();
    double bStart = m_curentRegion.getFromY();
    double scaleOfA = ((m_curentRegion.getToX() - aStart) / (double) width);
    double scaleOfB = ((m_curentRegion.getToY() - bStart) / (double) height);
    // Eviter un appel trop important de log2 : LOG2 (MN) = LOG2(M) + LOG2(N)
    unsigned int numberOfProducts;
    for(y = yStart; y < yEnd; ++y){
        yPos = y * width;
        for(x = xStart; x < xEnd; ++x){
            index = yPos + x;
            // Calcul la position de X/Y dans A/B
            a = aStart + x * scaleOfA;
            b = bStart + y * scaleOfB;
            expoLyap = 0;

            xn = X0;
            numberOfProducts = m_precision / 10;
            std::vector<double> product(numberOfProducts);
            for(i = 0; i< numberOfProducts; ++i){
                product[i] = 1;
                for (j = i*numberOfProducts; j<(i+1)*numberOfProducts; ++j){
                    rn = m_sequence[j] == 'A' ? a : b;
                    xn = rn * xn * (1 - xn);
                    product[i] *= (fabs(rn * (1 - 2 * xn)));
                }
                product[i] = log2(product[i]);
                expoLyap+= product[i];
            }

            m_exponents[index] = expoLyap / m_precision;
        }
    }
}

// Permet de modifier la taille de la fractale de Lyapunov
// lorsque l'on modifie la taille de la fenêtre
void Lyapunov::onResized(unsigned int newWidth, unsigned int newHeight){
    SDL_Rect newPos;
    newPos.w = newPos.h = (int) (newWidth < newHeight ? newWidth : newHeight);
    newPos.x = (int) ((newWidth >> 1u) - ((unsigned int) newPos.w >> 1u));
    newPos.y = (int) ((newHeight >> 1u) - ((unsigned int) newPos.h >> 1u));
    setTexturePosition(newPos);
    //blitTexture();
    //updateScreen();
}

// Permet de gérer les différents évenements liés à la souris
void Lyapunov::onMouseClick(unsigned int x, unsigned int y, unsigned int button){
    switch(button){
        // Un Clic Gauche sur la souris permet de zoomer dans la fractale
        case SDL_BUTTON_LEFT:{
            m_lastPosition.emplace(m_curentRegion);
            Region newRegion = getRegion((int) x - 200, (int) x + 200, (int) y - 200, (int) y + 200);
            generate(newRegion);
        }
            break;

            // Un Clic Droit permet de dézoomer grâce à la pile
        case SDL_BUTTON_RIGHT:{
            if(m_lastPosition.empty()){
                return;
            }
            Region dezoom = m_lastPosition.top();
            m_lastPosition.pop();
            generate(dezoom);
        }
            break;
        default:
            break;
    }

}

// Appelée au moment où le pointeur de la souris est en mouvement
// Affiche la région de la fractale dans lzquelle on zoome
void Lyapunov::onMouseMove(unsigned int x, unsigned int y){
    long time = getCurrentTime();
    if(time - m_lastMove < 16){
        return;
    }
    m_lastMove = time;
    blitTexture();
    drawRect((int) x - 200, (int) y - 200, 400, 400);
    updateScreen();
}

// S'occupe de la gestion des différents événements : clavier/souris/quit
void Lyapunov::startLoop(){
    eventLoop();
}

// Appelée au moment où la molette de la souris est en action
void Lyapunov::onMouseWheel(){

}

// Permet de déterminer la couleur actuelle
void Lyapunov::onTick(){
    /*if(!m_stopColor){
        m_currentColor = (360 + (m_currentColor - 5 % 360)) % 360;
        std::cout << m_currentColor << std::endl;
        updatePixels();
        blitTexture();
        updateScreen();
    }*/
}
// Gère les différents évenements liés au clavier
void Lyapunov::onKeyboard(int c){
    switch(c){
        // Appuyer sur ESPACE permet d'arrêter(reprendre) le défilement
        // des couleurs dans la fractale : Effet Arc En Ciel
        case SDLK_SPACE:
            m_stopColor = !m_stopColor;
            break;

    }
}

int main(int argc, char* argv[]){
    (void) argc, (void) argv;
    // Nécessaire à l'utilisation d'une fenêtre gtk
    Gtk::Main app(argc, argv);
    Menu m=Menu();
    // Ouvre le menu m
    Gtk::Main::run(m);

      // Recupère les saisies dans le fichier de config
    if (m.writeFile() == 0){
     Lyapunov lyapunov(1400, 1000, 1000, 1000);
     lyapunov.generate();
     lyapunov.startLoop();
     return EXIT_SUCCESS;
  }
  else {
    return -1;
  }
}
