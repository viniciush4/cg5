#ifndef CAMEERABOMBA_H
#define CAMEERABOMBA_H

class CameraBomba {
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
    CameraBomba(double x, double y, double z, double look_x, double look_y, double look_z,double up_x,double up_y,double up_z);
    void update(double x, double y, double z, double look_x, double look_y, double look_z,double up_x,double up_y,double up_z);
    void record();
    void changeCamera(int angle, int w, int h);
    virtual ~CameraBomba();
private:

};

#endif /* CAMEERABOMBA_H */

