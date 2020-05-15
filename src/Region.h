//
// Created by Antoine on 09/03/2020.
//

#ifndef LYAPUNOV_REGION_H
#define LYAPUNOV_REGION_H

#include <ostream>
#include <iostream>

// Cette classe permet de créer des "régions" : une zone comprise entre 2 points
class Region {

    double fromX, toX, fromY, toY;

public:
    friend std::ostream& operator<<(std::ostream& os, const Region& region);

    Region(double fromX, double toX, double fromY, double toY);

    double getFromX() const;

    double getToX() const;

    double getFromY() const;

    double getToY() const;
};


#endif //LYAPUNOV_REGION_H
