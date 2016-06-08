#include <iostream>
#include "modules.hpp"

Modules::Modules(CatomType ct):type(ct) {
    camera=NULL;
    currentColorId = 0;
}

Modules::~Modules() {
    delete camera;
}

uint16_t Modules::getColorId(const Color &color) {
    // seach color in tab
    vector<Color>::const_iterator ci = tabColors.begin();
    uint16_t num=0;
    while (ci!=tabColors.end() && !(*(ci)==color)) {
        num++;
        ci++;
    }
    if (ci==tabColors.end()) {
        tabColors.push_back(color);
        num = currentColorId++;
    }
    return num;
}


Modules *loadXml(const string &title, CatomType ct) {
  	TiXmlDocument *xmlDoc = new TiXmlDocument(title.c_str());

	bool isLoaded = xmlDoc->LoadFile();

	if ( !isLoaded) {
		std::cerr << "Could not load configuration file :" << title.c_str() << std::endl;
		exit(EXIT_FAILURE);
	} else {
		TiXmlNode *xmlWorldNode = xmlDoc->FirstChild("world");
		if (xmlWorldNode) {
			std::cerr << "\033[1;34m  " << title.c_str() << " successfully loaded "<< "\033[0m" << std::endl;
		} else {
			std::cerr << "\033[1;31m" << "Could not find root 'world' element in configuration file" << "\033[0m" << std::endl;
			exit(1);
		}
	}

    Modules *modules;
    switch (ct) {
        case C3D:
            modules = new Catoms3D();
        break;
        case BB:
            modules = new BlinkyBlocks();
        break;
    }

	/* reading the xml file */
	TiXmlNode *node = xmlDoc->FirstChild("world");
	if (node) {
		TiXmlElement* worldElement = node->ToElement();
		const char *attr= worldElement->Attribute("gridSize");
		int lx,ly,lz;
		if (attr) {
			string str=attr;
			int pos1 = str.find_first_of(','),
				pos2 = str.find_last_of(',');
			lx = atoi(str.substr(0,pos1).c_str());
			ly = atoi(str.substr(pos1+1,pos2-pos1-1).c_str());
			lz = atoi(str.substr(pos2+1,str.length()-pos1-1).c_str());
			std::cerr << "grid size : " << lx << " x " << ly << " x " << lz << std::endl;
			modules->gridSize.set(lx,ly,lz);
		} else {
			std::cerr << "WARNING No grid size in XML file" << std::endl;
		}
		attr=worldElement->Attribute("windowSize");
		if (attr) {
			string str=attr;
	 		int pos = str.find_first_of(',');
			modules->screenWidth = atoi(str.substr(0,pos).c_str());
			modules->screenHeight = atoi(str.substr(pos+1,str.length()-pos-1).c_str());
		}
	} else {
		std::cerr << "ERROR : NO world in XML file" << std::endl;
		exit(1);
	}

	// loading the camera parameters
	TiXmlNode *nodeConfig = node->FirstChild("camera");
	if (nodeConfig) {
		TiXmlElement* cameraElement = nodeConfig->ToElement();
		const char *attr=cameraElement->Attribute("target");
		double def_near=0.1,def_far=1500;
		float angle=45.0;
		modules->camera = new Camera(0,0,100);
		if (attr) {
			string str(attr);
			int pos1 = str.find_first_of(','),
			pos2 = str.find_last_of(',');
			Vecteur target;
			target.pt[0] = atof(str.substr(0,pos1).c_str());
			target.pt[1] = atof(str.substr(pos1+1,pos2-pos1-1).c_str());
			target.pt[2] = atof(str.substr(pos2+1,str.length()-pos1-1).c_str());
			modules->camera->setTarget(target);
		}
		attr=cameraElement->Attribute("angle");
		if (attr) {
			angle = (float)atof(attr);
			modules->camera->setAngle(angle);
		}
		attr=cameraElement->Attribute("directionSpherical");
		if (attr) {
			string str(attr);
			int pos1 = str.find_first_of(','),
			pos2 = str.find_last_of(',');
			float az,ele,dist;
			az = -90.0f+(float)atof(str.substr(0,pos1).c_str());
			ele = (float)atof(str.substr(pos1+1,pos2-pos1-1).c_str());
			dist = (float)atof(str.substr(pos2+1,str.length()-pos1-1).c_str());
			modules->camera->setDirection(az,ele);
			modules->camera->setDistance(dist);
			az = (float)(dist*sin(angle*M_PI/180.0));
			def_near = dist-az;
			def_far = dist+az;
		}
		attr=cameraElement->Attribute("near");
		if (attr) {
			def_near = atof(attr);
		}
		attr=cameraElement->Attribute("far");
		if (attr) {
			def_far = atof(attr);
		}
		modules->camera->setNearFar(def_near,def_far);
	}

	// loading the spotlight parameters
	nodeConfig = node->FirstChild("spotlight");
	if (nodeConfig) {
		Vecteur target;
		float az=0,ele=60,dist=1000,angle=50;
		double def_near=10.0,def_far=1500.0;
		TiXmlElement* lightElement = nodeConfig->ToElement();
		const char *attr=lightElement->Attribute("target");
		if (attr) {
			string str(attr);
			int pos1 = str.find_first_of(','),
			pos2 = str.find_last_of(',');
			target.pt[0] = atof(str.substr(0,pos1).c_str());
			target.pt[1] = atof(str.substr(pos1+1,pos2-pos1-1).c_str());
			target.pt[2] = atof(str.substr(pos2+1,str.length()-pos1-1).c_str());
		}
		attr=lightElement->Attribute("directionSpherical");
		if (attr) {
			string str(attr);
			int pos1 = str.find_first_of(','),
			pos2 = str.find_last_of(',');
			az = -90.0f+(float)atof(str.substr(0,pos1).c_str());
			ele = (float)atof(str.substr(pos1+1,pos2-pos1-1).c_str());
			dist = (float)atof(str.substr(pos2+1,str.length()-pos1-1).c_str());
		}
		attr=lightElement->Attribute("angle");
		if (attr) {
			angle = (float)atof(attr);
		}
		attr=lightElement->Attribute("near");
		if (attr) {
			def_near = (float)atof(attr);
		}
		attr=lightElement->Attribute("far");
		if (attr) {
			def_far = atof(attr);
		}
		if (modules->camera) modules->camera->setLightParameters(target,az,ele,dist,angle,def_near,def_far);
	}

	TiXmlNode *nodeBlock = node->FirstChild("blockList");
	if (nodeBlock) {
       	modules->loadModules(nodeBlock);
	} else // end if(nodeBlock)
	{ cerr << "no Block List" << std::endl;
	}

	return modules;
}

void Modules::writeHeaders(ofstream &fout) {
// entete
    fout << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
    << "<COLLADA xmlns=\"http://www.collada.org/2005/11/COLLADASchema\" version=\"1.4.1\">\n"
    << "<asset>\n"
    << "    <contributor>\n"
    << "        <author>Benoit Piranda</author>\n"
    << "        <authoring_tool>ConvertXml2Collada</authoring_tool>\n"
    << "    </contributor>\n"
    << "    <unit name=\"cm\" meter=\"0.01\"/>\n"
    << "    <up_axis>Z_UP</up_axis>\n"
    << "  </asset>\n";

// camera
    fout << "<library_cameras>\n"
    << "    <camera id=\"Camera-camera\" name=\"Camera\">\n"
    << "      <optics>\n"
    << "        <technique_common>\n"
    << "          <perspective>\n"
    << "            <xfov sid=\"xfov\">" << camera->getAngle() << "</xfov>\n"
    << "            <aspect_ratio>" << double(screenWidth)/double(screenHeight) << "</aspect_ratio>\n"
    << "            <znear sid=\"znear\">" << camera->getNear() << "</znear>\n"
    << "            <zfar sid=\"zfar\">" << camera->getFar() << "</zfar>\n"
    << "          </perspective>\n"
    << "        </technique_common>\n"
    << "      </optics>\n"
    << "    </camera>\n"
    << "</library_cameras>\n"
    << "<library_lights>\n"
    << "    <light id=\"Lamp-light\" name=\"Lamp\">\n"
    << "      <technique_common>\n"
    << "        <point>\n"
    << "          <color sid=\"color\">1 1 1</color>\n"
    << "          <constant_attenuation>1</constant_attenuation>\n"
    << "          <linear_attenuation>0</linear_attenuation>\n"
    << "          <quadratic_attenuation>0.00111109</quadratic_attenuation>\n"
    << "        </point>\n"
    << "      </technique_common>\n"
    << "    </light>\n"
    << "</library_lights>\n";

    fout << "<library_images/>\n";

// materials
    fout << "<library_effects>\n";
    vector<Color>::const_iterator cc = tabColors.begin();
    uint16_t num=0;
    while (cc!=tabColors.end()) {
        fout << "    <effect id=\"mat" << num << "-effect\">\n"
        << "      <profile_COMMON>\n"
        << "        <technique sid=\"common\">\n"
        << "          <phong>\n"
        << "            <emission>\n"
        << "              <color sid=\"emission\">0 0 0 1</color>\n"
        << "            </emission>\n"
        << "            <ambient>\n"
        << "              <color sid=\"ambient\">0 0 0 1</color>\n"
        << "            </ambient>\n"
        << "            <diffuse>\n"
        << "              <color sid=\"diffuse\">" << *cc << "</color>\n"
        << "            </diffuse>\n"
        << "            <specular>\n"
        << "              <color sid=\"specular\">0.1 0.1 0.1 1</color>\n"
        << "            </specular>\n"
        << "            <shininess>\n"
        << "              <float sid=\"shininess\">1</float>\n"
        << "            </shininess>\n"
        << "            <index_of_refraction>\n"
        << "              <float sid=\"index_of_refraction\">1</float>\n"
        << "            </index_of_refraction>\n"
        << "          </phong>\n"
        << "        </technique>\n"
        << "      </profile_COMMON>\n"
        << "    </effect>\n";
        num++;
        cc++;
    }
    fout << "</library_effects>\n";
    fout << "<library_materials>\n";
    cc = tabColors.begin();
    num=0;
    while (cc!=tabColors.end()) {
        fout << "    <material id=\"mat" << num << "-material\" name=\"mat"<< num << "\">\n"
        << "      <instance_effect url=\"#mat" << num << "-effect\"/>\n"
        << "    </material>\n";
        cc++;
        num++;
    }
    fout << "</library_materials>\n";

}
