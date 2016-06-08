#ifndef COLOR_H_
#define COLOR_H_

#include <iostream>
#include <fstream>
#include <cmath>
#ifndef _WIN
#include <memory.h>
#endif

class Color {
public :
    float rgba[4];
    Color();
    Color(float r,float g,float b,float a=1.0);

    void set(float r,float g,float b,float a=1.0);
    inline const float operator[](const int i) const { return rgba[i]; };
    friend std::ostream& operator<<(std::ostream& f,const Color &c);
};

bool operator==(const Color &a, const Color &b);

const Color WHITE(1.0f,1.0f,1.0f);
const Color RED(1.0f,0.0f,0.0f);
const Color GREEN(0.0f,1.0f,0.0f);
const Color LIGHTGREEN(0.75f,1.0f,0.25f);
const Color BLUE(0.0f,0.0f,1.0f);
const Color YELLOW(1.0f,1.0f,0.0f);
const Color CYAN(0.0f,1.0f,1.0f);
const Color MAGENTA(1.0f,0.0f,1.0f);
const Color LIGHTBLUE(173/255.0,216/255.0,230/255.0);
const Color GOLD(1.0,215/255.0,0);
const Color PINK(1.0,192/255.0,203/255.0);
const Color GREY(0.5,0.5,0.5);
const Color LIGHTGREY(0.75,0.75,0.75);
const Color DARKGREY(0.25,0.25,0.25);
const Color ORANGE(1.0,0.64706,0.0);
const Color DARKORANGE(1.0,0.549,0.0);


#endif // COLOR_H_
