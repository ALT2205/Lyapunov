//
// Created by Antoine on 09/03/2020.
//

#include "Region.h"

Region::Region(double fromX, double toX, double fromY, double toY) : fromX(fromX), toX(toX), fromY(fromY), toY(toY){}

double Region::getFromX() const{
    return fromX;
}

double Region::getToX() const{
    return toX;
}

double Region::getFromY() const{
    return fromY;
}

double Region::getToY() const{
    return toY;
}

std::ostream& operator<<(std::ostream& os, const Region& region){
    os << "fromX: " << region.fromX << " toX: " << region.toX << " fromY: " << region.fromY << " toY: " << region.toY;
    return os;
}
