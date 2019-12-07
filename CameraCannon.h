/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CameraCannon.h
 * Author: 2014100467
 *
 * Created on 21 de Novembro de 2018, 16:08
 */

#ifndef CAMERACANNON_H
#define CAMERACANNON_H

class CameraCannon {
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
    
public:
    CameraCannon(double x, double y, double z, double look_x, double look_y, double look_z,double up_x,double up_y,double up_z);
    void update(double x, double y, double z,double heightCannon,double y_angle, double z_angle,double player_angle);
    void draw();
    void record();
    virtual ~CameraCannon();
private:

};

#endif /* CAMERACANNON_H */

