#ifndef CAMERACANHAO_H
#define CAMERACANHAO_H

class CameraCanhao {
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
    CameraCanhao(double x, double y, double z, double look_x, double look_y, double look_z,double up_x,double up_y,double up_z);
    void update(double x, double y, double z,double heightCannon,double y_angle, double z_angle,double player_angle);
    void draw();
    void record();
    void changeCamera(int angle, int w, int h);
    virtual ~CameraCanhao();
private:

};

#endif /* CAMERACANHAO_H */

