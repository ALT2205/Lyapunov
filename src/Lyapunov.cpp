#include "Lyapunov.h"

// Constructeur de la classe Lyapunov
Lyapunov::Lyapunov(unsigned int lyapunovWidth, unsigned int lyapunovHeight)
        : WindowManager(), m_exponents(lyapunovWidth * lyapunovHeight), m_size(),
          m_lastPosition{}{
    // Ouverture en mode lecture du fichier de configuration
    updateSettings();
    generateSequence();
    m_size.w = (int) lyapunovWidth;
    m_size.h = (int) lyapunovHeight;
    initRender(m_size);
}

void Lyapunov::updateSettings(){
    std::ifstream file("config.txt");
    std::string str;
    int r, g, b, i = 0, precis;
    // On recupère d'abord les couleurs stockées dans un  tableau de Pixels(Uint32)
    while(i < 4 && file >> str >> r >> g >> b){
        m_colorLyap[i] = ((unsigned int)r << 16u) + ((unsigned int)g << 8u) + b;
        i++;
    }
    // Création des tableaux contenant les variations de couleurs en fonction des bornes
    setColorScale(0, m_colorLyap[1], m_colorLyap[0]); //Neg
    setColorScale(3, m_colorLyap[3], m_colorLyap[2]); //Pos
    // On recupère ensite la précision souhaitée
    file >> str >> precis;
    m_precision = precis;
    // On récupère finalement la sequence
    std::string seq;
    file >> str >> seq;
    m_sequence = seq;
}

// Renvoie la région sur le plan Lyapunov (entre 0 et 4)
// à partir de la région de l'écran
Region Lyapunov::getRegion(int fromX, int toX, int fromY, int toY){
    SDL_Rect texturePosition = getTexturePosition();
    Region region{
            //Si x est négatif, alors 0, si x dépasse de la texture, alors la largeur de la texture
            //Ensuite on divise par la largeur de la texture, on multiplie par la largeur actuelle du plan de Lyapunov
            //Et on ajoute l'origine
            (double) (fromX < texturePosition.x ? 0 : fromX > texturePosition.x + texturePosition.w ? texturePosition.w
                                                                                                    : fromX -
                                                                                                      texturePosition.x) /
            (double) texturePosition.w * (m_currentRegion.getToX() - m_currentRegion.getFromX()) +
            m_currentRegion.getFromX(),
            (double) (toX < texturePosition.x ? 0 : toX > texturePosition.x + texturePosition.w ? texturePosition.w :
                                                    toX - texturePosition.x) / (double) texturePosition.w *
            (m_currentRegion.getToX() - m_currentRegion.getFromX()) +
            m_currentRegion.getFromX(),
            (double) (fromY < texturePosition.y ? 0 : fromY > texturePosition.y + texturePosition.h ? texturePosition.h
                                                                                                    : fromY -
                                                                                                      texturePosition.y) /
            (double) texturePosition.h * (m_currentRegion.getToY() - m_currentRegion.getFromY()) +
            m_currentRegion.getFromY(),
            (double) (toY < texturePosition.y ? 0 : toY > texturePosition.y + texturePosition.h ? texturePosition.h :
                                                    toY - texturePosition.y) / (double) texturePosition.h *
            (m_currentRegion.getToY() - m_currentRegion.getFromY()) +
            m_currentRegion.getFromY()};
    return region;
}

// Définie la couleur d'un pixel dans l'espace colorimétrique RGB
void Lyapunov::setPixelRGB(std::vector<Uint32>& pixels, unsigned int index,
                           unsigned int r, unsigned int g, unsigned int b){
    pixels[index] = (r << 16u) + (g << 8u) + b;
}

// Met dans trois cases de colorScale l'intervalle entre deux couleurs
// et 6 cases plus loin, les valeurs sur lesquelles doivent être appliqués les variations
void Lyapunov::setColorScale(int tab, int max, int min){
    int currMax, currMin;
    for(int i = 2; i >= 0; i--){
        currMax = max % 256;
        currMin = min % 256;
        colorScale[tab + i] = currMax - currMin;
        colorScale[tab + 6 + i] = currMin;
        max = (max - currMax) / 256;
        min = (min - currMin) / 256;
    }
}


/*Modifie la couleur des pixels  dans un tableau en
fonction de la valeur de l'exposant associé à la même
position dans le tableau des exposants 2D*/
void Lyapunov::updatePixels(){
    std::vector<Uint32> pixels(m_size.w * m_size.h);
    int red, green, blue, choixTab;
    double diviseur;
    for(int i = 0, size = m_size.w * m_size.h; i < size; ++i){
        double exponent = m_exponents[i];
        if(exponent > 0){
            choixTab = 3;
            diviseur = maxExpo;
        } else {
            choixTab = 0;
            diviseur = minExpo;
        }
        exponent = exponent / diviseur;
        red = (int) (colorScale[choixTab] * exponent) + colorScale[choixTab + 6];
        green = (int) (colorScale[choixTab + 1] * exponent) + colorScale[choixTab + 7];
        blue = (int) (colorScale[choixTab + 2] * exponent) + colorScale[choixTab + 8];
        setPixelRGB(pixels, i, red, green, blue);
    }
    updateTexture(pixels);
}


// Génération de la séquence en fonction de la précision voulue
// et de la séquence initiale
void Lyapunov::generateSequence(){
    std::string sequence;
    bool error = false;
    for(char i : m_sequence){
        switch(i){
            case 'A' :
            case 'B' :
                continue;
            default :
                error = true;
        }
        //Si on arrive ici, alors on a quitté le switch, donc la séquence n'est pas valide
        break;
    }
    if(m_sequence.empty() || error){
        std::cout
                << "An error in the construction of the sequence has been detected. Sequence must contains only A and B. Default Sequence : AB"
                << std::endl;
        m_sequence = "AB";
    }
    sequence = m_sequence;
    //La séquence est multiplié autant de fois que nécesaire par rapport à la précision
    while((int)m_sequence.length() < m_precision){
        m_sequence += sequence;
    }
}


// Génère la fractale de Lyapunov dans une région donnée
void Lyapunov::generate(Region region){
    //Si la région est en dehors des limites, on l'ajuste
    if(region.getFromX() < 0 || region.getToX() > 4 || region.getFromY() < 0 || region.getToY() > 4){
        double fx = region.getFromX(), tx = region.getToX(), fy = region.getFromY(), ty = region.getToY(), length =
                tx - fx;
        if(fx < 0){
            fx = 0;
            tx = length;
        } else if(tx > 4){
            tx = 4;
            fx = 4 - length;
        }
        if(fy < 0){
            fy = 0;
            ty = length;
        } else if(ty > 4){
            ty = 4;
            fy = 4 - length;
        }
        m_currentRegion = Region(fx, tx, fy, ty);
    } else {
        m_currentRegion = Region{region};
    }
    if(m_sequence.empty()){
        generateSequence();
    }
    /*Cette partie de la fonction est responsable du multi-threading
    nécessaire à la création des fractales de Lyapunov
    Nombre de threads en fonction du nombre de threads du CPU
    On garde un thread libre pour gérer d'eventuels problèmes*/
    unsigned int nbThread = std::thread::hardware_concurrency();
    // Vector de threads afin de pouvoir géré le multi-threading
    std::vector<std::thread> threads(nbThread);
    // Création des différents threads générant une partie de la fractale de Lyapunov
    for(unsigned int i = 0; i < nbThread; i++){
        threads[i] = std::thread(&Lyapunov::generatePart, this, 0,
                                 i * m_size.w / nbThread, m_size.w,
                                 (i + 1) * m_size.h / nbThread);
    }
    /* Permet de synchroniser les différentes opérations gérées dans les threads.
       Met également fin aux threads.*/
    for(auto& th : threads){
        th.join();
    }
    updatePixels();
    // Tracer du rectangle affichant la zone Zoomée
    drawZoom();
}

// Permet de générer une partie de la fractale de Lyapunov
// Entre différentes coordonnées. Utile pour le multi-threading
// Calcul de l'exposant de Lyapunov : https://en.wikipedia.org/wiki/Lyapunov_fractal#Algorithm_for_generating_Lyapunov_fractals
void Lyapunov::generatePart(int xStart, int yStart, int xEnd, int yEnd){
    //Utilisation de variable local pour améliorer la performance
    int width = m_size.w, height = m_size.h, x, y, yPos, index, i;
    double a, b, expoLyap, xn, rn;
    minExpo = 0;
    maxExpo = 0;
    // Echelle d'espacement entre chaque a/b pour x/y
    double aStart = m_currentRegion.getFromX(),
            bStart = m_currentRegion.getFromY(),
            scaleOfA = ((m_currentRegion.getToX() - aStart) / (double) width),
            scaleOfB = ((m_currentRegion.getToY() - bStart) / (double) height),
            expo;
    // Eviter un appel trop important de log2 : LOG2 (MN) = LOG2(M) + LOG2(N)
    for(y = yStart; y < yEnd; ++y){
        yPos = y * width;
        for(x = xStart; x < xEnd; ++x){
            i = 0;
            index = yPos + x;
            // Calcul la position de X/Y dans A/B
            a = aStart + x * scaleOfA;
            b = bStart + y * scaleOfB;
            expoLyap = 0;

            xn = X0;
            while(i < m_precision){
                expo = 1;
                while(expo < 1e100){
                    rn = m_sequence[i] == 'A' ? a : b;
                    xn = rn * xn * (1 - xn);
                    expo *= (fabs(rn * (1 - 2 * xn)));
                    ++i;
                    if(i >= m_precision){
                        break;
                    }
                }
                expo = log2(expo);
                expoLyap += expo;
            }
            expoLyap /= m_precision;
            if(expoLyap < -30){
                expoLyap = minExpo;
            } else if(expoLyap > 30){
                expoLyap = maxExpo;
            }
            m_exponents[index] = expoLyap;
            minExpo = (expoLyap < minExpo) ? expoLyap : minExpo;
            maxExpo = (expoLyap > maxExpo) ? expoLyap : maxExpo;
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
}

// Permet de gérer les différents évenements liés à la souris
void Lyapunov::onMouseClick(int mouseX, int mouseY, int button){
    switch(button){
        case SDL_BUTTON_LEFT:{
            // Un Clic Gauche sur la souris permet de zoomer dans la fractale
            m_lastPosition.emplace(m_currentRegion);
            //Décalage des coordonnées
            int shift = (int) ((unsigned int) m_zoomPrecision >> 1u);
            int x = mouseX - shift, y = mouseY - shift, w = m_zoomPrecision, h = w;
            validateRegion(x, y, w, h);
            m_currentRegion = getRegion(x, x + w, y, y + h);
            generate(m_currentRegion);
        }
            break;
        case SDL_BUTTON_RIGHT:{
            // Un Clic Droit permet de dézoomer grâce à la pile
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
void Lyapunov::onMouseMove(){
    long time = getCurrentTime();
    //60 fps
    if(time - m_lastMove < 16){
        return;
    }
    m_lastMove = time;
    drawZoom();
}

// S'occupe de la gestion des différents événements : clavier/souris/quit
void Lyapunov::startLoop(){
    eventLoop();
}

// Appelée au moment où la molette de la souris est en action
void Lyapunov::onMouseWheel(int amount){
    int max = (int) (getTexturePosition().w * (3 / 4.0));
    m_zoomPrecision += amount * 10;
    //Limites
    if(m_zoomPrecision < 5){
        m_zoomPrecision = 5;
    } else if(m_zoomPrecision > max){
        m_zoomPrecision = max;
    }
    drawZoom();
}

void Lyapunov::onKeyboardDown(int c){
    switch(c){
        /* Différents déplacements (Pas optimisé) */
        case SDLK_RIGHT:{
            double distance = (m_currentRegion.getToX() - m_currentRegion.getFromX()) / 2;
            m_currentRegion = {m_currentRegion.getFromX() + distance,
                               m_currentRegion.getToX() + distance, m_currentRegion.getFromY(),
                               m_currentRegion.getToY()};
            generate(m_currentRegion);
        }
            break;
        case SDLK_LEFT:{
            double distance = (m_currentRegion.getToX() - m_currentRegion.getFromX()) / (-2);
            m_currentRegion = {m_currentRegion.getFromX() + distance,
                               m_currentRegion.getToX() + distance, m_currentRegion.getFromY(),
                               m_currentRegion.getToY()};
            generate(m_currentRegion);
        }
            break;
        case SDLK_DOWN:{
            double distance = (m_currentRegion.getToY() - m_currentRegion.getFromY()) / 2;
            m_currentRegion = {m_currentRegion.getFromX(),
                               m_currentRegion.getToX(), m_currentRegion.getFromY() + distance,
                               m_currentRegion.getToY() + distance};
            generate(m_currentRegion);
        }
            break;
        case SDLK_UP:{
            double distance = (m_currentRegion.getToY() - m_currentRegion.getFromY()) / (-2);
            m_currentRegion = {m_currentRegion.getFromX(),
                               m_currentRegion.getToX(), m_currentRegion.getFromY() + distance,
                               m_currentRegion.getToY() + distance};
            generate(m_currentRegion);
        }
            break;
        case SDLK_RETURN:
            screenShot();
            break;
        case SDLK_ESCAPE:{
            std::string seq = m_sequence;
            int precis = m_precision;
            Menu k = Menu();
            k.setColorButton();
            Gtk::Main::run(k);
            updateSettings();
            if(m_sequence.length() == 0){
                m_sequence = seq;
            }
            generateSequence();
            if(seq != m_sequence || precis != m_precision){
                generate(m_currentRegion);
            }
            updatePixels();
            blitTexture();
            updateScreen();
        }
            break;
        default:
            break;

    }
    blitTexture();
    updateScreen();
}

void Lyapunov::drawZoom(){
    SDL_Rect mouse = getMousePosition();
    int shift = (int)((unsigned int)m_zoomPrecision >> 1u);
    int x = mouse.x - shift, y = mouse.y - shift, w = m_zoomPrecision, h = w;
    validateRegion(x, y, w, h);
    blitTexture();
    drawRect(x, y, w, h);
    updateScreen();
}

void Lyapunov::validateRegion(int& x, int& y, int& w, int& h){
    SDL_Rect texture = getTexturePosition();
    if(x < texture.x){
        x = texture.x;
    }
    if(x + w > texture.x + texture.w){
        x = texture.x + texture.w - w;
    }
    if(y < texture.y){
        y = texture.y;
    }
    if(y + h > texture.y + texture.h){
        y = texture.y + texture.h - h;
    }
}

int main(int argc, char* argv[]){
    (void) argc, (void) argv;
    // Nécessaire à l'utilisation d'une fenêtre gtk
    Gtk::Main app(argc, argv);
    Menu m = Menu();
    // Ouvre le menu m
    Gtk::Main::run(m);
    m.writeFile();

    // Recupère les saisies dans le fichier de config
    Lyapunov lyapunov(1000, 1000);
    lyapunov.generate();
    lyapunov.startLoop();
    return EXIT_SUCCESS;
}
