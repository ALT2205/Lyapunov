//
// Created by Antoine on 04/03/2020.
//

#ifndef LYAPUNOV_COORDINATES_H
#define LYAPUNOV_COORDINATES_H

class Coordinates {
    double m_x, m_y;
public:

    explicit Coordinates(double x = 0, double y = 0);

    double getX() const;

    void setX(double x);

    double getY() const;

    void setY(double y);
};


#endif //LYAPUNOV_COORDINATES_H
