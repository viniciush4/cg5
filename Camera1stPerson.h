/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Camera1stPerson.h
 * Author: 2014100467
 *
 * Created on 21 de Novembro de 2018, 16:08
 */

#ifndef CAMEERA1STPERSON_H
#define CAMEERA1STPERSON_H

class Camera1stPerson {
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
    Camera1stPerson(double x, double y, double z, double look_x, double look_y, double look_z,double up_x,double up_y,double up_z);
    void update(double x, double y, double z,double player_angle);
    void record();
    virtual ~Camera1stPerson();
private:

};

#endif /* CAMEERA1STPERSON_H */

