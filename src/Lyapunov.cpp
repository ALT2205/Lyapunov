#include "Lyapunov.h"

/* Couleur : Jaune si Exposant < 0
   Couleur : Bleu si Exposant > 0
   https://en.wikipedia.org/wiki/Lyapunov_fractal
   Calcul de l'xposant de Lyapunov via https://www.youtube.com/watch?v=8xZyA09zRXY

    Degradé au format brut : Pourquoi ne pas passer par un fichier ?*/

Lyapunov::Lyapunov(unsigned int windowWidth, unsigned int windowHeight,
                   unsigned int lyapunovWidth, unsigned int lyapunovHeight)
        : WindowManager(windowWidth, windowHeight), m_exponents(lyapunovWidth * lyapunovHeight), m_size(),
          m_lastPosition{}{
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

/*
 * Renvoie la région sur le plan Lyapunov (entre 0 et 4)
 * à partir de la région de l'écran
 * */
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

void Lyapunov::setPixelRGB(std::vector<Uint32>& pixels, unsigned int index,
                           unsigned int r, unsigned int g, unsigned int b){
    pixels[index] = (r << 16u) + (g << 8u) + b;
}

/*
 * Algorithme de conversion: https://fr.wikipedia.org/wiki/Teinte_Saturation_Valeur#Conversion_de_TSV_vers_RVB
 * Faut-il déclarer des variables ou bien passer directement en paramètre au détriment de la lisibilité ?
 * Y a-t-il un impact sur les performances en déclarant si appelé beaucoup de fois en peu de temps ?
 *
 * Utilisée surtout pour faire le test arc-en-ciel
 */
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

void Lyapunov::updatePixels(){
    std::vector<Uint32> pixels(m_size.w * m_size.h);
    for(int i = 0, size = m_size.w * m_size.h; i < size; ++i){
        double exponent = m_exponents[i];
        int greenLayer = ((int) (210 + exponent * 50) >= 0) ? (int) (210 + exponent * 50) : 0;
        int redLayer = ((int) (255 + exponent * 52) >= 100) ? (int) (255 + exponent * 52) : 100;
        int blueLayer = ((int) (255 - exponent * 200) >= 0) ? (int) (255 - exponent * 200) : 0;
        //int h = m_currentColor;
        //setPixelHSV(pixels, i, greenLayer + h, 1, 1);
        if(exponent < -6){
            setPixelRGB(pixels, i, 0, 0, 0);
        } else if(exponent <= 0){
            setPixelRGB(pixels, i, 0, greenLayer, 0);
        } else if(exponent > 0){
            setPixelRGB(pixels, i, 0, 0, blueLayer);
        } else if(exponent >= 1){
            setPixelRGB(pixels, i, 0, 0, 0);
        }
    }
    updateTexture(pixels);
}

void Lyapunov::generateSequence(){
    //std::cout << "Entrez la sequence de A-B\n";
    std::string sequence;
    //std::cin >> seq;
    sequence = "BBABBABBABBABAABBBBA";
    while(m_sequence.length() < m_precision){
        m_sequence += sequence;
    }
}

void Lyapunov::generate(Region region){
    m_curentRegion = Region{region};
    if(region.getFromX() < 0 || region.getToX() > 4 || region.getFromY() < 0 || region.getToY() > 4){
        throw std::domain_error("Invalid domain to generate Lyapunov");
    }
    if(m_sequence.empty()){
        generateSequence();
    }
    //if(aStart > aEnd){
    //    double change = aStart;
    //    aStart = aEnd;
    //    aEnd = change;
    //}
    //if(bStart > bEnd){
    //    double change = bStart;
    //    bStart = bEnd;
    //    bEnd = change;
    //}
    // Creation du nombre de threads en fonction du nombre de threads de l'ordinateur
    unsigned int nbThread = std::thread::hardware_concurrency();
    std::vector<std::thread> threads(nbThread);
    for(unsigned int i = 0; i < nbThread; i++){
        threads[i] = std::thread(&Lyapunov::generatePart, this, 0,
                                 i * m_size.w / nbThread, m_size.w,
                                 (i + 1) * m_size.h / nbThread);
    }
    for(auto& th : threads){
        th.join();
    }
    updatePixels();
    blitTexture();
    SDL_Rect mousePos = getMousePosition();
    drawRect((int) mousePos.x - 200, (int) mousePos.y - 200, 400, 400);
    updateScreen();
    //std::cout << "Generation finie\n";
}

void Lyapunov::generatePart(unsigned int xStart, unsigned int yStart, unsigned int xEnd, unsigned int yEnd){
    //Utilisation de variable local pour améliorer la performance
    unsigned int width = m_size.w, height = m_size.h;
    unsigned int i, x, y, yPos, index;
    double a, b, expoLyap, xn, rn;
    // Echelle d'espacement entre chaque a/b pour x/y
    double aStart = m_curentRegion.getFromX();
    double bStart = m_curentRegion.getFromY();
    double scaleOfA = ((m_curentRegion.getToX() - aStart) / (double) width);
    double scaleOfB = ((m_curentRegion.getToY() - bStart) / (double) height);
    //std::cout << aStart << " - " << bStart << "\n";
    //std::cout << m_curentRegion.getToX() << " / " << m_curentRegion.getBottomRight().getY() << "\n";
    for(y = yStart; y < yEnd; ++y){
        //std::cout << y * 100 / width << "%" << std::endl;
        yPos = y * width;
        for(x = xStart; x < xEnd; ++x){
            index = yPos + x;
            // Calcul la position de X/Y dans A/B
            a = aStart + x * scaleOfA;
            b = bStart + y * scaleOfB;
            expoLyap = 0;
            xn = X0;
            for(i = 0; i < m_precision; ++i){
                // Choix entre A ou B selon le current Char
                rn = m_sequence[i] == 'A' ? a : b;
                // Calcul de Xn+1
                xn = rn * xn * (1 - xn);
                expoLyap += log2(fabs(rn * (1 - 2 * xn)));
            }
            m_exponents[index] = expoLyap / m_precision;
        }
    }
}

void Lyapunov::onResized(unsigned int newWidth, unsigned int newHeight){
    SDL_Rect newPos;
    newPos.w = newPos.h = (int) (newWidth < newHeight ? newWidth : newHeight);
    newPos.x = (int) ((newWidth >> 1u) - ((unsigned int) newPos.w >> 1u));
    newPos.y = (int) ((newHeight >> 1u) - ((unsigned int) newPos.h >> 1u));
    setTexturePosition(newPos);
    blitTexture();
    updateScreen();
}

void Lyapunov::onMouseClick(unsigned int x, unsigned int y, unsigned int button){
    switch(button){
        case SDL_BUTTON_LEFT:{
            m_lastPosition.emplace(m_curentRegion);
            Region newRegion = getRegion((int) x - 200, (int) x + 200, (int) y - 200, (int) y + 200);
            generate(newRegion);
        }
            break;
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

void Lyapunov::startLoop(){
    eventLoop();
}

void Lyapunov::onMouseWheel(){

}

void Lyapunov::onTick(){
    /*if(!m_stopColor){
        m_currentColor = (360 + (m_currentColor - 5 % 360)) % 360;
        std::cout << m_currentColor << std::endl;
        updatePixels();
        blitTexture();
        updateScreen();
    }*/
}

void Lyapunov::onKeyboard(int c){
    switch(c){
        case SDLK_SPACE:
            m_stopColor = !m_stopColor;
            break;
    }
}

int main(int argc, char* argv[]){
    (void) argc, (void) argv;
    Lyapunov lyapunov(1400, 1000, 1000, 1000);
    lyapunov.generate();
    lyapunov.startLoop();
    return EXIT_SUCCESS;
}
