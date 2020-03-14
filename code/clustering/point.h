#pragma once

const int NOISE = -2;   //the point is defined as a noise point
const int NOT_CLASSIFIED = -1;  //the point has not been specified into one group

class Point {
public:
    double x, y, z, vx, vy, vz; //x,y,z: position of the viewport; vx, vy, vz: speed of user's view in x, y, z direction
    int ptsCnt, cluster;    //ptsCnt: number of points in the neighbourhood; cluster: the index of the group to which the point belongs
    double w;   //weight for distance and velocity in calculation
    /*
    Point(double x, double y, double z, double vx, double vy, double vz, int ptsCnt, int cluster, double w) {
        this->x = x;
        this->y = y;
        this->z = z;
        this->vx = vy;
        this->vy = vy;
        this->vz = vz;
        this->ptsCnt = ptsCnt;
        this->cluster = cluster;
        this->w = w;
    }
    */
    
    //get the weighted distance between two points
    double getDis(const Point& ot) {
        return w*sqrt((x - ot.x) * (x - ot.x) + (y - ot.y) * (y - ot.y) + (z - ot.z) * (z - ot.z))+(1-w)*sqrt((vx - ot.vx) * (vx - ot.vx) + (vy - ot.vy) * (vy - ot.vy) + (vz - ot.vz) * (vz - ot.vz));
    }
};