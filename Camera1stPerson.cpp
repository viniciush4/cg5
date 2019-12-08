/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   Camera1stPerson.cpp
 * Author: 2014100467
 *
 * Created on 21 de Novembro de 2018, 16:08
 */

#include <cmath>
#include <GL/glu.h>
#include <GL/glut.h>
#include <iostream>

#include "Camera1stPerson.h"

using namespace std;


Camera1stPerson::Camera1stPerson(double x, double y, double z, double look_x, double look_y, double look_z,double up_x,double up_y,double up_z){
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

void Camera1stPerson::update(double x, double y, double z, double z_angle, double y_angle){

    this->xt = x + this->x*cos(z_angle*(M_PI/180.0));     
    this->yt = y + this->x*sin(z_angle*(M_PI/180.0));
    this->zt = z + this->z + this->x*sin(y_angle*(M_PI/180.0)); 

    this->look_x = xt + 5*cos(z_angle*(M_PI/180.0));
    this->look_y = yt + 5*sin(z_angle*(M_PI/180.0));
    this->look_z = zt + 5*sin(y_angle*(M_PI/180.0));
}

void Camera1stPerson::record(){

   gluLookAt(this->xt,this->yt ,this->zt,                  //Position
             this->look_x,this->look_y,this->look_z,  //look point
             this->up_x,this->up_y,this->up_z);       //UpVector


}

void Camera1stPerson::draw(){
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

Camera1stPerson::~Camera1stPerson() {
}
