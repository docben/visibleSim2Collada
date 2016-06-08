#include "color.hpp"

Color::Color() {
    memset(rgba,0,4*sizeof(float));
}

Color::Color(float r,float g,float b,float a) {
    rgba[0]=r;
    rgba[1]=g;
    rgba[2]=b;
    rgba[3]=a;
}

void Color::set(float r,float g, float b, float a) {
    rgba[0]=r;
    rgba[1]=g;
    rgba[2]=b;
    rgba[3]=a;
}

// écriture d'une couleur dans un flux
std::ostream& operator<<(std::ostream& f,const Color&p)
{ f << p.rgba[0] << " " << p.rgba[1] << " " << p.rgba[2] << " " << p.rgba[3];
  return f;
}

bool operator==(const Color &a, const Color &b) {
    return ((a.rgba[0] == b.rgba[0]) && (a.rgba[1] == b.rgba[1]) && (a.rgba[2] == b.rgba[2]) && (a.rgba[3] == b.rgba[3]));
}
