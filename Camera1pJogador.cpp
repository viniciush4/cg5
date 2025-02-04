#include <cmath>
#include <GL/glu.h>
#include <GL/glut.h>
#include <iostream>

#include "Camera1pJogador.h"

using namespace std;


Camera1pJogador::Camera1pJogador(double x, double y, double z, double look_x, double look_y, double look_z,double up_x,double up_y,double up_z){
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

void Camera1pJogador::update(double x, double y, double z, double z_angle, double y_angle){

//Tentativa 1
    //this->xt = x + this->x*cos(z_angle*(M_PI/180.0));     
    //this->yt = y + this->x*sin(z_angle*(M_PI/180.0));
    //this->zt = z + this->z + this->x*sin(y_angle*(M_PI/180.0)); 

//Tentativa 2
/*
    this->xt = x + (this->x*cos(z_angle*(M_PI/180.0)))/1.8;     
    this->yt = y + (this->x*sin(z_angle*(M_PI/180.0)))/1.8;
    this->zt = z + this->z + 1.2 + (this->x*sin(y_angle*(M_PI/180.0)))/1.8; 

    this->look_x = xt + 2*cos(z_angle*(M_PI/180.0));
    this->look_y = yt + 2*sin(z_angle*(M_PI/180.0));
    this->look_z = zt + 2*sin(y_angle*(M_PI/180.0));
*/
//Tentativa 3
    this->xt = x + (this->x*cos(z_angle*(M_PI/180.0)))/15;     
    this->yt = y + (this->x*sin(z_angle*(M_PI/180.0)))/15;
    this->zt = z + this->z + 4.5 + (this->x*sin(y_angle*(M_PI/180.0)))/15; 

    this->look_x = xt + 5*cos(z_angle*(M_PI/180.0));
    this->look_y = yt + 5*sin(z_angle*(M_PI/180.0));
    this->look_z = zt + 5*sin(y_angle*(M_PI/180.0));

}

void Camera1pJogador::record(){

   gluLookAt(
        this->xt,this->yt ,this->zt,
        this->look_x,this->look_y,this->look_z,
        this->up_x,this->up_y,this->up_z
    );
}

void Camera1pJogador::changeCamera(int angle, int w, int h){
    glMatrixMode (GL_PROJECTION);

    glLoadIdentity();    

    gluPerspective (angle,
            (GLfloat)w / (GLfloat)h, 0.5, 5000.0);

    glMatrixMode (GL_MODELVIEW);
}

void Camera1pJogador::draw(){
    glPushMatrix();
        GLfloat mat_ambient_r[] = { 1.0, 0.0, 0.0, 1.0 };
        glColor3fv(mat_ambient_r);
        glTranslatef(xt,yt,zt);
        glutSolidCube(0.5);
    glPopMatrix();

    glPushMatrix();
        glColor3fv(mat_ambient_r);
        glTranslatef(look_x,look_y,look_z);
        glutSolidCube(0.5);
    glPopMatrix();

}

Camera1pJogador::~Camera1pJogador() {
}
