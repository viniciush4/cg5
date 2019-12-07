/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Camera.h
 * Author: eduardo
 *
 * Created on 4 de Novembro de 2018, 15:41
 */

#ifndef CAMERA3DPERSON_H
#define CAMERA3DPERSON_H

class Camera3dPerson {
    double x;
    double y;
    double z;
    
    double xt;
    double yt;
    double zt;       
    
    double look_x;
    double look_y;
    double look_z;
    
    double up_x;
    double up_y;
    double up_z;
    
    double y_angle;
    double z_angle;
    
public:
    Camera3dPerson(double x, double y, double z, double look_x, double look_y, double look_z,double up_x,double up_y,double up_z);
    void record();
    void update(double x, double y, double z,double y_angle, double z_angle);
    void changeCamera(int angle, int w, int h);
    
    void setDist(double dist);
    
    double getZAngle() const;
    double getYAngle() const;

    void setZAngle(double z_angle);
    
    virtual ~Camera3dPerson();

};

#endif /* CAMERA3DPERSON_H */

