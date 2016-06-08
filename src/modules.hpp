#ifndef MODULES_HPP_INCLUDED
#define MODULES_HPP_INCLUDED

#if defined(_WIN32)
#include <stdint.h> 
typedef uint8_t u_int8_t;
typedef uint16_t u_int16_t;
typedef uint32_t u_int32_t;

#include <math.h>
#ifndef M_SQRT1_2
#define M_SQRT1_2	0.70710678118654752440084436210485
#endif

#endif

#include "vecteur.hpp"
#include "camera.hpp"
#include "color.hpp"
#include "cell3DPosition.hpp"
#include "TinyXML/tinyxml.h"
#include <vector>

enum CatomType { BB=0, SM=1, C2D=2, C3D=3 };

class Modules {
public :
    std::vector<Color> tabColors;
    CatomType type;
    Cell3DPosition gridSize;
    Camera *camera;
    int screenWidth,screenHeight;
    uint16_t currentColorId;

    Modules(CatomType ct);
    ~Modules();
    virtual void loadModules(TiXmlNode *nodeBlock)=0;
    virtual void createDae(const std::string &title,const std::string &model)=0;
    uint16_t getColorId(const Color& color);
    void writeHeaders(ofstream &fout);
};

class BlinkyBlock {
public :
    uint64_t id;
    Cell3DPosition position;
    uint16_t idColor;

    BlinkyBlock(uint64_t c_id,const Cell3DPosition &c_position,uint16_t c_color):id(c_id),position(c_position),idColor(c_color) {};
    Matrice getMatrix(const Vecteur &bs) const;
};

class BlinkyBlocks:public Modules {
    std::vector<BlinkyBlock> tabBlinkyBlocks;
    Vecteur blockSize;
public:
    BlinkyBlocks():Modules(BB) {};
    ~BlinkyBlocks() {};

    void loadModules(TiXmlNode *nodeBlock);
    void createDae(const std::string &title,const std::string &model);
};


class Catom3D {
public :
    uint64_t id;
    Cell3DPosition position;
    uint16_t idColor;

    Catom3D(uint64_t c_id,const Cell3DPosition &c_position,uint16_t c_color):id(c_id),position(c_position),idColor(c_color) {};
    Matrice getMatrix(const Vecteur &bs) const;
};

class Catoms3D:public Modules {
    std::vector<Catom3D> tabCatoms3D;
    Vecteur blockSize;
public:
    Catoms3D():Modules(C3D) {};
    ~Catoms3D() {};

    void loadModules(TiXmlNode *nodeBlock);
    void createDae(const std::string &title,const std::string &model);
};

Modules *loadXml(const string &titre,CatomType ct);

#endif // MODULES_HPP_INCLUDED
