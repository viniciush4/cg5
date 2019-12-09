#include <cmath>
#include <GL/glu.h>
#include <GL/glut.h>
#include <iostream>

#include "CameraCanhao.h"

using namespace std;


CameraCanhao::CameraCanhao(double x, double y, double z, double look_x, double look_y, double look_z,double up_x,double up_y,double up_z){
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

void CameraCanhao::update(double x, double y, double z,double heightCannon,double y_angle, double z_angle,double player_angle){    

    this->xt = x + this->x*cos(player_angle*(M_PI/180.0)) + heightCannon*sin(y_angle*(M_PI/180.0)) * cos((z_angle + player_angle)*(M_PI/180.0));
    this->yt = y + this->x*sin(player_angle*(M_PI/180.0)) + heightCannon*sin(y_angle*(M_PI/180.0)) * sin((z_angle + player_angle)*(M_PI/180.0));
    this->zt = z + this->z + heightCannon*cos(y_angle*(M_PI/180.0));    

    this->look_x = xt + 15*cos((z_angle + player_angle)*(M_PI/180.0));
    this->look_y = yt + 15*sin((z_angle + player_angle)*(M_PI/180.0));
    this->look_z = zt - 15*sin(y_angle*(M_PI/180.0));

}

void CameraCanhao::record(){

   gluLookAt(this->xt,this->yt ,this->zt,                  //Position
             this->look_x,this->look_y,this->look_z,  //look point
             this->up_x,this->up_y,this->up_z);       //UpVector

}

void CameraCanhao::draw(){
    glPushMatrix();
        glTranslatef(xt,yt,zt);
        glutSolidCube(0.5);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(look_x,look_y,look_z);
        glutSolidCube(0.5);
    glPopMatrix();

}

CameraCanhao::~CameraCanhao() {
}
