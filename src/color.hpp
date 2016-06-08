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
const Color LIGHTBLUE(173.0f/255.0f,216.0f/255.0f,230.0f/255.0f);
const Color GOLD(1.0f,215.0f/255.0f,0.0f);
const Color PINK(1.0f,192.0f/255.0f,203.0f/255.0f);
const Color GREY(0.5f,0.5f,0.5f);
const Color LIGHTGREY(0.75f,0.75f,0.75f);
const Color DARKGREY(0.25f,0.25f,0.25f);
const Color ORANGE(1.0f,0.64706f,0.0f);
const Color DARKORANGE(1.0f,0.549f,0.0f);


#endif // COLOR_H_
