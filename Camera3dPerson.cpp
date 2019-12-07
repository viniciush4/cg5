/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Camera.cpp
 * Author: eduardo
 * 
 * Created on 4 de Novembro de 2018, 15:41
 */

#include "Camera3dPerson.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <cmath>
#include <iostream>

using namespace std;

Camera3dPerson::Camera3dPerson(double x, double y, double z, double look_x, double look_y, double look_z,double up_x,double up_y,double up_z) {
    this->x = x;
    this->y = y;
    this->z = z;
    
    this->xt = 0;
    this->yt= 0;
    this->zt = 0;
    
    this->look_x = look_x;
    this->look_y = look_y;
    this->look_z = look_z;
    
    this->up_x = up_x;
    this->up_y = up_y;
    this->up_z = up_z;
    
    this->z_angle = 0;
    this->y_angle = 30;    
}

void Camera3dPerson::setDist(double dist){    
    if(this->x <= 0){
        this->x += dist;
    }else{
        this->x = 0;
    }    
}

void Camera3dPerson::update(double x, double y, double z,double y_angle, double z_angle){
    
    this->xt = x;
    this->yt = y;
    this->zt = z;
    
    this->y_angle = y_angle;
    this->z_angle = z_angle;           
    
    if(abs(this->y_angle) > 60){
        if(this->y_angle > 0)
          this->y_angle = 60;
        else{
          this->y_angle = -60;
        }
    }  

    if(abs(this->z_angle) > 180){
        if(this->z_angle > 0)
          this->z_angle = 180;
        else{
          this->z_angle = -180;
        }
    }    
    
}

double Camera3dPerson::getYAngle() const{
    return this->y_angle;
}

double Camera3dPerson::getZAngle() const{
    return this->z_angle;
}

void Camera3dPerson::setZAngle(double z_angle){
    this->z_angle = z_angle;
}

void Camera3dPerson::record(){
    
    
    gluLookAt(this->x,this->y,this->z,                  //Position
              this->look_x,this->look_y,this->look_z,  //look point
              this->up_x,this->up_y,this->up_z);       //UpVector
    

    glRotatef(this->y_angle,0,-1,0); 
    glRotatef(this->z_angle,0,0,-1);          
    glTranslatef(-xt,-yt,-zt);        

    
    
      
    
//    glutSolidCube(0.2);

}

void Camera3dPerson::changeCamera(int angle, int w, int h){
    glMatrixMode (GL_PROJECTION);

    glLoadIdentity();    

    gluPerspective (angle,
            (GLfloat)w / (GLfloat)h, 3, 5000.0);

    glMatrixMode (GL_MODELVIEW);
}

Camera3dPerson::~Camera3dPerson() {
}

