#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>

#include "modules.hpp"

int main(int argc,char **argv) {
    std::string titleSrc,titleDest,titleModel;
    CatomType catomType;

    if (argc>3) {
        titleSrc = std::string(argv[1]);
        titleDest = std::string(argv[2]);
        std::string str(argv[3]);
        if (str=="C3D") {
            catomType=C3D;
            titleModel=std::string("catom3Dgeometry.xml");
        } else
        if (str=="C2D") {
            catomType=C2D;
            titleModel=std::string("blinkyBlockgeometry.xml");
        } else
        if (str=="BB") catomType=BB; else
        if (str=="SM") catomType=SM;
        if (argc>5 && strcmp(argv[4],"-g")==0) {
            titleModel = std::string(argv[5]);
        }
    } else {
        std::cerr << "usage convertXml2Collada src dest catomType -g geometry.xml" << std::endl;
        return -1;
    }

    Modules *modules = loadXml(titleSrc,catomType);

    modules->createDae(titleDest,titleModel);


    delete modules;
    return 0;
}
