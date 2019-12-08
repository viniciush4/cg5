#ifndef CAMERA3DBASE_H
#define CAMERA3DBASE_H

class Camera3dBase {
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
    double z_angle_inicial;
    
public:
    Camera3dBase(double x, double y, double z, double look_x, double look_y, double look_z,double up_x,double up_y,double up_z);
    void record();
    void update(double x, double y, double z,double y_angle, double z_angle);
    void changeCamera(int angle, int w, int h);
    
    void setDist(double dist);
    
    double getZAngle() const;
    double getYAngle() const;

    void setZAngle(double z_angle);
    void draw();
    
    virtual ~Camera3dBase();

};

#endif /* CAMERA3DBASE_H */

