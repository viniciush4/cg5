#include <cmath>
#include <GL/glu.h>
#include <GL/glut.h>
#include <iostream>

#include "CameraBomba.h"

using namespace std;


CameraBomba::CameraBomba(double x, double y, double z, double look_x, double look_y, double look_z,double up_x,double up_y,double up_z){
    this->x = x;
    this->y = y;
    this->z = z;

    this->xt = 0;
    this->yt = 0;
    this->zt = 0;

    this->look_x = look_x;
    this->look_y = look_y;
    this->look_z = look_z;

    this->up_x = up_x;
    this->up_y = up_y;
    this->up_z = up_z;
}

void CameraBomba::update(double x, double y, double z, double look_x, double look_y, double look_z,double up_x,double up_y,double up_z){

    this->x = x;     
    this->y = y;
    this->z = z; 

    this->xt = x;     
    this->yt = y;
    this->zt = z; 

    this->look_x = look_x;
    this->look_y = look_y;
    this->look_z = look_z;

    this->up_x = up_x;
    this->up_y = up_y;
    this->up_z = up_z;
}

void CameraBomba::record(){

    gluLookAt(
        this->xt,this->yt ,this->zt,
        this->look_x,this->look_y,this->look_z,
        this->up_x,this->up_y,this->up_z
    );
}

void CameraBomba::changeCamera(int angle, int w, int h){
    glMatrixMode (GL_PROJECTION);

    glLoadIdentity();    

    gluPerspective (angle,
            (GLfloat)w / (GLfloat)h, 5, 5000.0);

    glMatrixMode (GL_MODELVIEW);
}

CameraBomba::~CameraBomba() {
}
