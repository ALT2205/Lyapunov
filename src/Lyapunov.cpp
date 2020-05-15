#include "Lyapunov.h"

// Constructeur de la classe Lyapunov
Lyapunov::Lyapunov(unsigned int lyapunovWidth, unsigned int lyapunovHeight)
        : WindowManager(), m_exponents(lyapunovWidth * lyapunovHeight), m_size(),
          m_lastPosition{}{
    updateSettings();
    generateSequence();
    m_size.w = (int) lyapunovWidth;
    m_size.h = (int) lyapunovHeight;
    initRender(m_size);
}

/*
 * Mis a jour des couleurs, de la séquence, et de la précision de la génération des fractales.
 */
void Lyapunov::updateSettings(){
    //Ouverture en mode lecture du fichier de configuration
    std::ifstream file("config.txt");
    std::string str;
    int r, g, b, i = 0, precision;
    // On recupère d'abord les couleurs stockées dans un  tableau de Pixels(Uint32)
    while(i < 4 && file >> str >> r >> g >> b){
        m_colorLyap[i++] = ((unsigned int) r << 16u) + ((unsigned int) g << 8u) + b;
    }
    // Création des tableaux contenant les variations de couleurs en fonction des bornes
    setColorScale(0, m_colorLyap[1], m_colorLyap[0]); //Neg
    setColorScale(3, m_colorLyap[3], m_colorLyap[2]); //Pos
    // On recupère ensite la précision souhaitée
    file >> str >> precision;
    m_precision = precision;
    // On récupère finalement la sequence
    std::string seq;
    file >> str >> seq;
    m_sequence = seq;
}

/*
 * Convertit les coordonnées écrans en coordonnées entre 0 et 4
 */
Region Lyapunov::getRegion(int fromX, int toX, int fromY, int toY) const{
    SDL_Rect texturePosition = getTexturePosition();
    Region region{
            //Si x est négatif, alors 0, si x dépasse de la texture, alors la largeur de la texture
            //Ensuite on divise par la largeur de la texture, on multiplie par la largeur actuelle du plan de Lyapunov
            //Et on ajoute l'origine
            (double) (fromX < texturePosition.x ? 0 : fromX > texturePosition.x + texturePosition.w ?
                    texturePosition.w : fromX - texturePosition.x) / (double) texturePosition.w *
                    (m_currentRegion.getToX() - m_currentRegion.getFromX()) + m_currentRegion.getFromX(),
            (double) (toX < texturePosition.x ? 0 : toX > texturePosition.x + texturePosition.w ?
                    texturePosition.w : toX - texturePosition.x) / (double) texturePosition.w *
                    (m_currentRegion.getToX() - m_currentRegion.getFromX()) + m_currentRegion.getFromX(),
            (double) (fromY < texturePosition.y ? 0 : fromY > texturePosition.y + texturePosition.h ?
                    texturePosition.h : fromY - texturePosition.y) / (double) texturePosition.h *
                    (m_currentRegion.getToY() - m_currentRegion.getFromY()) + m_currentRegion.getFromY(),
            (double) (toY < texturePosition.y ? 0 : toY > texturePosition.y + texturePosition.h ?
                    texturePosition.h : toY - texturePosition.y) / (double) texturePosition.h *
                    (m_currentRegion.getToY() - m_currentRegion.getFromY()) + m_currentRegion.getFromY()};
    return region;
}

// Fonction pour extraire un dégradé RGB à partir de deux couleurs en Uint32
void Lyapunov::setColorScale(int tab, int max, int min){
    int currMax, currMin;
    for(int i = 2; i >= 0; --i){ // à chaque itération, une composante RGB différente
        currMax = max % 256;
        currMin = min % 256;
        // Met dans trois cases de colorScale la différence entre deux couleurs
        m_colorScale[tab + i] = currMax - currMin;
        // et 6 cases plus loin, les valeurs sur lesquelles doivent être appliqués les variations
        m_colorScale[tab + 6 + i] = currMin;
        max = (max - currMax) / 256;
        min = (min - currMin) / 256;
    }
}


/*
 * Modifie la couleur des pixels selon les exposants
 */
void Lyapunov::updatePixels() const{
    std::vector<Uint32> pixels(m_size.w * m_size.h);
    unsigned int choice;
    double divideBy, exponent;
    for(int i = 0, size = m_size.w * m_size.h; i < size; ++i){
        exponent = m_exponents[i];
        if(exponent > 0){
            //Cas où l'exposant positif : on prend comme diviseur l'exposant maximal et le tableau de couleur adapté
            choice = 3;
            divideBy = m_maxExpo;
        } else {
            //Cas où l'exposant négatif : on prend comme diviseur l'exposant minimal et le tableau de couleur adapté
            choice = 0;
            divideBy = m_minExpo;
        }
        // (exp/diviseur)* composante RGB qui varie + composante RGB sur laquelle agir
        exponent /= divideBy; // cela permet de créer un intervalle de couleurs
        pixels[i] = (((unsigned int) (m_colorScale[choice] * exponent) + m_colorScale[choice + 6]) << 16u) + //red
                    (((unsigned int) (m_colorScale[choice + 1] * exponent) + m_colorScale[choice + 7]) << 8u) + //green
                    (m_colorScale[choice + 2] * exponent) + m_colorScale[choice + 8]; //blue
    }
    updateTexture(pixels);
}

/*
 * Génération de la séquence
 */
void Lyapunov::generateSequence(){
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
        m_sequence = "";
        for(int i = 0, size = rand() % m_precision; i < size; ++i){
            m_sequence += (rand() % 2 == 0 ? "A" : "B");
        }
        std::cout << "Sequence aléatoire: " << m_sequence << std::endl;
    }
    std::string sequence = m_sequence;
    //La séquence est multipliée autant de fois que nécesaire par rapport à la précision
    while((int) m_sequence.length() < m_precision){
        m_sequence += sequence;
    }
}

/*
 * Génération des fractales selon une certaine région
 */
void Lyapunov::generate(Region region){
    //Si la région est en dehors des limites, on l'ajuste
    if(region.getFromX() < 0 || region.getToX() > 4 || region.getFromY() < 0 || region.getToY() > 4){
        double x1 = region.getFromX(),
                x2 = region.getToX(),
                y1 = region.getFromY(),
                y2 = region.getToY(),
                length = x2 - x1;
        if(x1 < 0){
            x1 = 0;
            x2 = length;
        } else if(x2 > 4){
            x2 = 4;
            x1 = 4 - length;
        }
        if(y1 < 0){
            y1 = 0;
            y2 = length;
        } else if(y2 > 4){
            y2 = 4;
            y1 = 4 - length;
        }
        m_currentRegion = Region(x1, x2, y1, y2);
    } else {
        m_currentRegion = region;
    }
    if(m_sequence.empty()){
        generateSequence();
    }
    m_minExpo = 0;
    m_maxExpo = 0;
    /* Cette partie de la fonction est responsable du multi-threading
       nécessaire à la création des fractales de Lyapunov
       Nombre de threads en fonction du nombre de threads du CPU*/
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

/*
 * Permet de générer une partie de la fractale de Lyapunov
 * Entre différentes coordonnées. Utile pour le multi-threading
 * Calcul de l'exposant de Lyapunov : https://en.wikipedia.org/wiki/Lyapunov_fractal#Algorithm_for_generating_Lyapunov_fractals
 */
void Lyapunov::generatePart(int xStart, int yStart, int xEnd, int yEnd){
    //Utilisation de variable local pour améliorer la performance
    int width = m_size.w, height = m_size.h, x, y, yPos, index, i;
    double a, b, expoLyap, xn, rn,
            aStart = m_currentRegion.getFromX(),
            bStart = m_currentRegion.getFromY(),
            scaleOfA = ((m_currentRegion.getToX() - aStart) / (double) width),
            scaleOfB = ((m_currentRegion.getToY() - bStart) / (double) height),
            expo, max = 1e100;
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
                //Éviter un appel trop important de log2 : LOG2 (MN) = LOG2(M) + LOG2(N)
                while(expo < max){
                    rn = m_sequence[i] == 'A' ? a : b;
                    xn = rn * xn * (1 - xn);
                    expo *= (fabs(rn * (1 - 2 * xn)));
                    if(++i >= m_precision){
                        break;
                    }
                }
                expoLyap += log2(expo);
            }
            /*Recherche des valeurs minimales et maximales pour fournir un intervalle d'exposants positifs et un négatifs
            pour établir un intervalle de couleurs*/
            expoLyap /= m_precision;
            /* Les valeurs négatives pouvant atteindre -infini, si une valeur est très importante négativement
            elle prend comme nouvelle valeur le minimum actuel pour un meilleur dégradé*/
            if(expoLyap < -10){
                expoLyap = m_minExpo;
            } else if(expoLyap > 10){
                expoLyap = m_maxExpo;
            }
            m_exponents[index] = expoLyap;
            if(expoLyap < m_minExpo){
                m_minExpo = expoLyap;
            }
            if(expoLyap > m_maxExpo){
                m_maxExpo = expoLyap;
            }
        }
    }
}

/*
 * Mets à jour la position de la texture lorsqu'on change la fenêtre de taille
 */
void Lyapunov::onResized(unsigned int newWidth, unsigned int newHeight){
    SDL_Rect newPos;
    newPos.w = newPos.h = (int) (newWidth < newHeight ? newWidth : newHeight);
    newPos.x = (int) ((newWidth >> 1u) - ((unsigned int) newPos.w >> 1u));
    newPos.y = (int) ((newHeight >> 1u) - ((unsigned int) newPos.h >> 1u));
    setTexturePosition(newPos);
}

/*
 * Appelé lors d'un clic
 * Gère le zoom
 */
void Lyapunov::onMouseClick(int mouseX, int mouseY, int button){
    switch(button){
        //Zoomer
        case SDL_BUTTON_LEFT:{
            m_lastPosition.emplace(m_currentRegion);
            //Décalage des coordonnées
            int shift = (int) ((unsigned int) m_zoomPrecision >> 1u);
            int x = mouseX - shift, y = mouseY - shift, w = m_zoomPrecision, h = w;
            validateRegion(x, y, w, h);
            m_currentRegion = getRegion(x, x + w, y, y + h);
            generate(m_currentRegion);
        }
            break;
            //Dézoomer
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

/*
 * Appelé lors d'un mouvement de souris
 * Utilisé pour afficher la zone où le zoom sera effectif
 */
void Lyapunov::onMouseMove(){
    long time = getCurrentTime();
    //60 fps
    if(time - m_lastMove < 16){
        return;
    }
    m_lastMove = time;
    drawZoom();
}

/*
 * Appelé lors d'une action sur la molette
 * Utilisé pour augmenter / réduire la puissance du zoom
 */
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

/*
 * Lance la boucle d'évènement
 */
void Lyapunov::startLoop(){
    eventLoop();
}

/*
 * Appelée lors d'un appui sur le clavier
 * Utilisé pour les déplacements, le zoom, la capture d'écran et le menu
 */
void Lyapunov::onKeyboardDown(int c){
    switch(c){
        // Différents déplacements
        // Une pression fait une distance d'une moitié de largeur d'une région
        case SDLK_RIGHT:{
            //Définition de la distance et du sens
            double distance = (m_currentRegion.getToX() - m_currentRegion.getFromX()) / 2;
            //Définition de la nouvelle région
            m_currentRegion = {m_currentRegion.getFromX() + distance,
                               m_currentRegion.getToX() + distance,
                               m_currentRegion.getFromY(),
                               m_currentRegion.getToY()};
            //Génération et affichage de la nouvelle region
            generate(m_currentRegion);
        }
            break;
        case SDLK_LEFT:{
            double distance = (m_currentRegion.getToX() - m_currentRegion.getFromX()) / 2;
            m_currentRegion = {m_currentRegion.getFromX() - distance,
                               m_currentRegion.getToX() - distance,
                               m_currentRegion.getFromY(),
                               m_currentRegion.getToY()};
            generate(m_currentRegion);
        }
            break;
        case SDLK_DOWN:{
            double distance = (m_currentRegion.getToY() - m_currentRegion.getFromY()) / 2;
            m_currentRegion = {m_currentRegion.getFromX(),
                               m_currentRegion.getToX(),
                               m_currentRegion.getFromY() + distance,
                               m_currentRegion.getToY() + distance};
            generate(m_currentRegion);
        }
            break;
        case SDLK_UP:{
            double distance = (m_currentRegion.getToY() - m_currentRegion.getFromY()) / 2;
            m_currentRegion = {m_currentRegion.getFromX(),
                               m_currentRegion.getToX(),
                               m_currentRegion.getFromY() - distance,
                               m_currentRegion.getToY() - distance};
            generate(m_currentRegion);
        }
            break;
        case SDLK_RETURN:
            screenShot();
            break;
        case SDLK_ESCAPE:{
            /*
             * On sauvegarde la séquence et la précision afin de détecter une différence avec les nouvelles entrées du menu.
             */
            std::string seq = m_sequence;
            int precis = m_precision;
            Menu k = Menu(m_precision);
            // On met à jour les couleurs avec les couleurs précedemment séléctionnées par l'utilisateur.
            k.setColorButton();
            Gtk::Main::run(k);
            updateSettings();
            // Si la séquence saisie est vide, alors on réutilise l'ancienne.
            if(m_sequence.length() == 0){
                m_sequence = seq;
            }
            generateSequence();
            //Si les deux séquences une fois générées sont différentes, ou la précison a été modifiée, alors on regénère les fractales à la région actuelle.
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

/*
 * Affiche un carré au blanc autour de la souris pour représenter le zoom effectif
 */
void Lyapunov::drawZoom(){
    SDL_Rect mouse = getMousePosition();
    int shift = (int) ((unsigned int) m_zoomPrecision >> 1u);
    int x = mouse.x - shift, y = mouse.y - shift, w = m_zoomPrecision, h = w;
    validateRegion(x, y, w, h);
    blitTexture();
    drawRect(x, y, w, h);
    updateScreen();
}

/*
 * Si la région spécifié sort de la texture, on la limite
 */
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
    srand(time(nullptr));
    (void) argc, (void) argv;
    // Nécessaire à l'utilisation d'une fenêtre gtk
    Gtk::Main app(argc, argv);
    Menu m = Menu();
    // Ouvre le menu m
    Gtk::Main::run(m);
    Lyapunov lyapunov(1000, 1000);
    lyapunov.generate();
    lyapunov.startLoop();
    return EXIT_SUCCESS;
}
