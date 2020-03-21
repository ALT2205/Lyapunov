//
// Created by Antoine on 09/03/2020.
//

#include "Region.h"

// Constructeur de la classe Région à partir de 4 coordonnées
Region::Region(double fromX, double toX, double fromY, double toY) : fromX(fromX), toX(toX), fromY(fromY), toY(toY){}

// Getter qui retourne la coordonnée X du premier point
double Region::getFromX() const{
    return fromX;
}

// Getter qui retourne la coordonnée X du second point
double Region::getToX() const{
    return toX;
}

// Getter qui retourne la coordonnée Y du premier point
double Region::getFromY() const{
    return fromY;
}

// Getter qui retourne la coordonnée Y du second point
double Region::getToY() const{
    return toY;
}

// Surcharge de l'opérateur <<. Permet d'afficher les régions.
std::ostream& operator<<(std::ostream& os, const Region& region){
    os << "fromX: " << region.fromX << " toX: " << region.toX << " fromY: " << region.fromY << " toY: " << region.toY;
    return os;
}
