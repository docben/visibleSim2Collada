#include <iostream>
#include "modules.hpp"

Matrice SmartBlock::getMatrix(const Vecteur &bs) const {
    Matrice mat;

    double sigma = 0.5*((int)position.pt[2]%2);
    mat.setTranslation((position.pt[0]+sigma)*bs[0],(position.pt[1]+sigma)*bs[1],position.pt[2]*bs[2]*M_SQRT1_2);
    return mat;
}

void SmartBlocks::loadModules(TiXmlNode *nodeBlock) {
    Color defaultColor=DARKGREY;
    TiXmlElement* element = nodeBlock->ToElement();
    uint64_t currentID=0;

    const char *attr= element->Attribute("color");
    if (attr) {
        string str(attr);
        int pos1 = str.find_first_of(','),
        pos2 = str.find_last_of(',');
        defaultColor.rgba[0] = (float)(atof(str.substr(0,pos1).c_str())/255.0);
        defaultColor.rgba[1] = (float)(atof(str.substr(pos1+1,pos2-pos1-1).c_str())/255.0);
        defaultColor.rgba[2] = (float)(atof(str.substr(pos2+1,str.length()-pos1-1).c_str())/255.0);
        std::cerr << "new default color :" << defaultColor << std::endl;
    }
    attr= element->Attribute("blocksize");
    if (attr) {
         string str(attr);
         int pos1 = str.find_first_of(','),
          pos2 = str.find_last_of(',');
         blockSize.pt[0] = atof(str.substr(0,pos1).c_str());
         blockSize.pt[1] = atof(str.substr(pos1+1,pos2-pos1-1).c_str());
         blockSize.pt[2] = atof(str.substr(pos2+1,str.length()-pos1-1).c_str());
         std::cerr << "blocksize =" << blockSize[0] << "," << blockSize[1] << "," << blockSize[2] << std::endl;
    }

/* Reading a block */
    TiXmlNode *block = nodeBlock->FirstChild("block");
    Cell3DPosition position;
    int orientation=0;
    Color color;
    bool master;
    while (block) {
       element = block->ToElement();
       color=defaultColor;
       master=false;
       orientation=0;
       // set the color
       attr = element->Attribute("color");
       if (attr) {
           string str(attr);
           int pos1 = str.find_first_of(','),
               pos2 = str.find_last_of(',');
           color.set((float)(atof(str.substr(0,pos1).c_str())/255.0),
                     (float)(atof(str.substr(pos1+1,pos2-pos1-1).c_str())/255.0),
                     (float)(atof(str.substr(pos2+1,str.length()-pos1-1).c_str())/255.0));
           std::cerr << "new color :" << color << std::endl;
        }
        // set the position
        attr = element->Attribute("position");
        if (attr) {
            string str(attr);
            int pos1 = str.find_first_of(','),
                pos2 = str.find_last_of(',');
            position.set(atoi(str.substr(0,pos1).c_str()),
                         atoi(str.substr(pos1+1,pos2-pos1-1).c_str()),
                         atoi(str.substr(pos2+1,str.length()-pos1-1).c_str()));
            std::cerr << "position : " << position << std::endl;
        }
        uint16_t idColor = getColorId(color);
        tabSmartBlocks.push_back(SmartBlock(currentID++,position,idColor));
        block = block->NextSibling("block");
    } // end while (block)
}

void SmartBlocks::createDae(const std::string& title,const std::string &model) {
    std::ofstream fout(title.c_str());

    writeHeaders(fout);

    char c;
    std::ifstream fin(model.c_str());
    if (fin.is_open()) {
        while (fin.get(c)) {
            fout << c;
        }
        fin.close();
    } else {
		cerr << "File not found: " << title.c_str() << endl;
    }
// controllers
    fout << "<library_controllers/>\n";
    Matrice mat = camera->getMatrix();
// visual scenes
    fout << "<library_visual_scenes>\n"
    << "    <visual_scene id=\"Scene\" name=\"Scene\">\n"
    << "    <node id=\"Camera\" name=\"Camera\" type=\"NODE\">\n"
    << "        <matrix sid=\"transform\">" << mat << "</matrix>\n"
    << "        <instance_camera url=\"#Camera-camera\"/>\n"
    << "    </node>\n"
    << "    <node id=\"Lamp\" name=\"Lamp\" type=\"NODE\">\n";
    mat = camera->getLightMatrix();
    fout << "        <matrix sid=\"transform\">" << mat << "</matrix>\n"
    << "        <instance_light url=\"#Lamp-light\"/>\n"
    << "      </node>\n";

    vector<SmartBlock>::const_iterator ci = tabSmartBlocks.begin();
    while (ci!=tabSmartBlocks.end()) {
        mat = (*ci).getMatrix(blockSize);
        fout << "      <node id=\"catom_origine\" name=\"catom_origine\" type=\"NODE\">\n"
        << "        <matrix sid=\"transform\">" << mat <<"</matrix>\n"
        << "        <instance_geometry url=\"#catom_origine-mesh\" name=\"catom_origine" << (*ci).id << "\">\n"
        << "          <bind_material>\n"
        << "            <technique_common>\n"
        << "              <instance_material symbol=\"mat" << (*ci).idColor << "-material\" target=\"#mat" << (*ci).idColor << "-material\"/>\n"
        << "            </technique_common>\n"
        << "          </bind_material>\n"
        << "        </instance_geometry>\n"
        << "      </node>" << endl;
        ci++;
    }
    fout << "    </visual_scene>\n"
    << "</library_visual_scenes>\n"
    << "<scene>\n"
    << "    <instance_visual_scene url=\"#Scene\"/>\n"
    << "</scene>\n"
    << "</COLLADA>" << endl;

    fout.close();
}
