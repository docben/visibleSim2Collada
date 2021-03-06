/***************************************************************************
  camera.cpp  -  gestion de la camera OpenGL
                             -------------------
    copyright            : (C) 2011 by Benoît Piranda
    email                : benoit.piranda@univ-fcomte.fr
FEMTO-ST/DISC / Universite de Franche-Comté
 ***************************************************************************/

#include "camera.hpp"
Camera::Camera(double t, double p, double d, double SX,double SY) {
	phi=p;
	theta=t;
	distance = d;
	sensibilityX=SX;
	sensibilityY=SY;
	targetMotion=false;
	updatePositionFromAngles();
}

void Camera::updatePositionFromAngles() {
	double OCamx = distance*cos(phi)*cos(theta),
		   OCamy = distance*cos(phi)*sin(theta),
		   OCamz = distance*sin(phi);
	position.pt[0] = target.pt[0] + OCamx;
	position.pt[1] = target.pt[1] + OCamy;
	position.pt[2] = target.pt[2] + OCamz;
	Xcam.set(OCamy,-OCamx,0);
	Xcam.normer_interne();
	Ycam.set(-OCamx*OCamz,-OCamy*OCamz,OCamx*OCamx+OCamy*OCamy);
	Ycam.normer_interne();
}

Matrice Camera::getMatrix() {
    Matrice mat,op;

    mat.setTranslation(target.pt[0],target.pt[1],target.pt[2]+distance);
    op.setRotationX(90.0-phi*180.0/M_PI);
    mat = op*mat;
    op.setRotationZ(90.0+theta*180.0/M_PI);
    mat = op*mat;
    return mat;
}


void Camera::mouseDown(int x, int y,bool tm) {
	mouse[0]=x;
	mouse[1]=y;
	targetMotion=tm;
}

void Camera::mouseUp(int x, int y) {
	if (targetMotion)
	{ target+=-0.1*(mouse[0]-x)*Xcam + 0.1*(y-mouse[1])*Ycam;
	} else
	{ theta += (mouse[0]-x)*sensibilityX;
	  phi -= (mouse[1]-y)*sensibilityY;
	  if (phi>M_PI/2) phi=M_PI/2;
	  else if (phi<-M_PI/2) phi=-M_PI/2;
	}
	targetMotion=false;
	updatePositionFromAngles();
}

void Camera::mouseMove(int x, int y) {
	if (targetMotion) {
		target+=-0.1*(mouse[0]-x)*Xcam + 0.1*(y-mouse[1])*Ycam;
	} else {
		theta += (mouse[0]-x)*sensibilityX;
		phi -= (mouse[1]-y)*sensibilityY;
		if (phi>M_PI/2) phi=M_PI/2;
		else if (phi<-M_PI/2) phi=-M_PI/2;
	}
	mouse[0]=x;
	mouse[1]=y;
  	updatePositionFromAngles();
}

void Camera::mouseZoom(double pas) {
	distance+=pas;
	updatePositionFromAngles();
}

void Camera::setLightParameters(const Vecteur &t, double th,double ph,double d,double angle,double nearplane,double farplane) {
	ls.target[0] = t[0];
	ls.target[1] = t[1];
	ls.target[2] = t[2];
	ls.theta=th*M_PI/180;
	ls.phi=ph*M_PI/180;
	ls.distance=d;
	ls.falloffAngle=angle;
	ls.near_plane=nearplane;
	ls.far_plane=farplane;
	ls.calcMatrices();
}

Matrice Camera::getLightMatrix() {
    Matrice mat,op;

    mat.setTranslation(ls.target[0],ls.target[1],ls.target[2]+ls.distance);
    op.setRotationX(90.0-ls.phi*180.0/M_PI);
    mat = op*mat;
    op.setRotationZ(90+ls.theta*180.0/M_PI);
    mat = op*mat;
    return mat;
}


void Camera::updateIntrinsics(double a,double rwh,double np,double fp) {
	angle=a;
	w_h = rwh;
	near_plane=np;
	far_plane=fp;
}

void Camera::mouseLightDown(int x, int y) {
	mouse[0]=x;
	mouse[1]=y;
}

void Camera::mouseLightUp(int x, int y) {
	ls.theta += (mouse[0]-x)*sensibilityX;
	ls.phi -= (mouse[1]-y)*sensibilityY;
	if (ls.phi>M_PI/2) ls.phi=M_PI/2;
	else if (ls.phi<-M_PI/2) ls.phi=-M_PI/2;

	ls.calcMatrices();
}

void Camera::mouseLightMove(int x, int y) {
	ls.theta += (mouse[0]-x)*sensibilityX;
	ls.phi -= (mouse[1]-y)*sensibilityY;
	if (ls.phi>M_PI/2) ls.phi=M_PI/2;
	else if (ls.phi<-M_PI/2) ls.phi=-M_PI/2;
	mouse[0]=x;
	mouse[1]=y;

	ls.calcMatrices();
}

void Camera::mouseLightZoom(double pas) {
	ls.distance+=GLfloat(pas);
	ls.calcMatrices();
}

LightSource::LightSource() {
	falloffAngle=60.0;
	near_plane=1.0;
	far_plane=10000.0;
	theta=phi=0;
	distance=1000;
}

void LightSource::calcMatrices() {
	dir[0] = -cos(phi)*cos(theta);
	dir[1] = -cos(phi)*sin(theta);
	dir[2] = -sin(phi);
	pos[0] = target[0] - distance*dir[0];
	pos[1] = target[1] - distance*dir[1];
	pos[2] = target[2] - distance*dir[2];
	pos[3]=1.0;
}

ostream& operator<<(ostream& f,const Camera &c)
{ f << "(" << c.phi*180.0/M_PI << "," << c.theta*180.0/M_PI << "," << c.distance << ")";
  return f;
}

