#ifndef CAMEERA1PJOGADOR_H
#define CAMEERA1PJOGADOR_H

class Camera1pJogador {
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
    Camera1pJogador(double x, double y, double z, double look_x, double look_y, double look_z,double up_x,double up_y,double up_z);
    void update(double x, double y, double z, double z_angle, double y_angle);
    void record();
    void draw();
    virtual ~Camera1pJogador();
private:

};

#endif /* CAMEERA1PJOGADOR */

