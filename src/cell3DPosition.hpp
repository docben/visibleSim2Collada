/*!
 * \file cell3DPosition.h
 * \brief 3D integer position in a grid
 * \date 29/01/2012
 * \author Benoît Piranda
 */

#ifndef CELL3DPOSITION_H
#define CELL3DPOSITION_H

#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

class Cell3DPosition {
public:
    short pt[3];
    Cell3DPosition();
    Cell3DPosition(short x,short y,short z);

    void set(short x,short y,short z);
    inline const short operator[](const int i) const { return pt[i]; };
    friend ostream& operator<<(ostream& f,const Cell3DPosition&p);
};

#endif // CELL3DPOSITION_H
