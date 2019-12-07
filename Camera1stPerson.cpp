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

void Camera1stPerson::update(double x, double y, double z,double player_angle){

    this->xt = x + this->x*cos(player_angle*(M_PI/180.0));     
    this->yt = y + this->x*sin(player_angle*(M_PI/180.0));
    this->zt = z + this->z; 

    this->look_x = xt + 5*cos((player_angle)*(M_PI/180.0));
    this->look_y = yt + 5*sin((player_angle)*(M_PI/180.0));
    this->look_z = zt;

}

void Camera1stPerson::record(){

   gluLookAt(this->xt,this->yt ,this->zt,                  //Position
             this->look_x,this->look_y,this->look_z,  //look point
             this->up_x,this->up_y,this->up_z);       //UpVector


}

Camera1stPerson::~Camera1stPerson() {
}
