//
// Created by Antoine on 04/03/2020.
//

#include "Coordinates.h"

Coordinates::Coordinates(double x, double y) : m_x(x), m_y(y){}

double Coordinates::getX() const{
    return m_x;
}

void Coordinates::setX(double x){
    Coordinates::m_x = x;
}

double Coordinates::getY() const{
    return m_y;
}

void Coordinates::setY(double y){
    Coordinates::m_y = y;
}


