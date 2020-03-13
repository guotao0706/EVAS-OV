#pragma once
#include <vector>
#include "point.h"

using namespace std;

class DBSCAN {
public:
    int n, minPts;
    double eps;
    vector<Point> points;
    int size;
    vector<vector<int> > adjPoints;
    vector<bool> visited;
    vector<vector<int> > cluster;
    int clusterIdx;

    DBSCAN(int n, double eps, int minPts, vector<Point> points) {
        this->n = n;    //number of groups(not used)
        this->eps = eps;    //radius of a neighbourhood
        this->minPts = minPts;  //minimum points in a neighbourhood
        this->points = points;
        this->size = (int)points.size();
        adjPoints.resize(size);
        this->clusterIdx = -1;
    }

    //run the clustering algorithm
    void run() {
        checkNearPoints();  //check neighbor point information of all points

        for (int i = 0; i < size; i++) {
            if (points[i].cluster != NOT_CLASSIFIED) continue;   //point has been clustered

            if (isCoreObject(i)) {   //a new core that hasn't been classfied
                dfs(i, ++clusterIdx);   //create a new group，and find all points belonging to this class using an iterative algorithm
            }
            else {
                points[i].cluster = NOISE;  //标记为噪声点，不属于任何一个group
            }
        }

        cluster.resize(clusterIdx + 1);
        for (int i = 0; i < size; i++) {
            if (points[i].cluster != NOISE) {
                cluster[points[i].cluster].push_back(i);
            }
        }
    }

    /***
    iterative algorithm to find all points belonging to the same class
    :param now: index of point
    :param c: index of group
    ***/
    void dfs(int now, int c) {
        points[now].cluster = c;    //
        if (!isCoreObject(now)) return;

        for (auto& next : adjPoints[now]) {
            if (points[next].cluster != NOT_CLASSIFIED) continue;
            dfs(next, c);
        }
    }

    //Using traversal to check the distance between each point and other points, 
    //and then include points that meet the radius conditions into the neighborhood.
    void checkNearPoints() {
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (i == j) continue;
                if (points[i].getDis(points[j]) <= eps) {
                    points[i].ptsCnt++;
                    adjPoints[i].push_back(j);
                }
            }
        }
    }

    /***
    check if one point is a core point
    :param idx: index of the point
    ***/
    bool isCoreObject(int idx) {
        return points[idx].ptsCnt >= minPts;
    }

    vector<vector<int> > getCluster() {
        return cluster;
    }

    //calculate the average point of each group
    vector<Point> getClusterAverage() {
        vector<Point> average(cluster.size());
        if (clusterIdx >= 0) {
            for (int i = 0; i < cluster.size(); i++) {
                Point np = { 0,0,0,0,0,0,0,0,0 };
                //ptsCnt, cluster and w has no meaning in a central point's calculation
                np.ptsCnt = points.at(0).ptsCnt;
                np.cluster = points.at(0).cluster;
                np.w = points.at(0).w;
                for (int j = 0; j < cluster.at(i).size(); j++) {
                    np.x += points.at(cluster.at(i).at(j)).x;
                    np.y += points.at(cluster.at(i).at(j)).y;
                    np.z += points.at(cluster.at(i).at(j)).z;
                    np.vx += points.at(cluster.at(i).at(j)).vx;
                    np.vy += points.at(cluster.at(i).at(j)).vy;
                    np.vz += points.at(cluster.at(i).at(j)).vz;
                }
                np.x /= cluster.at(i).size();
                np.y /= cluster.at(i).size();
                np.z /= cluster.at(i).size();
                np.vx /= cluster.at(i).size();
                np.vy /= cluster.at(i).size();
                np.vz /= cluster.at(i).size();
                average.at(i) = np;
            }
            return average;
        }
    }

    //get the MSE of points by calculating the difference of each point's viewport's postion 
    //and its group's average point's position
    double getMSE() {
        vector<Point> average = getClusterAverage();    //calculate the average point of each group
        double error_x = 0;
        double error_y = 0;
        double error_z = 0;
        int clustered_num = 0;
        for (int i = 0; i < points.size(); i++) {
            if (points.at(i).cluster == NOISE) continue;    //noise points belong to no group
            else {
                error_x += points.at(i).x - average.at(points.at(i).cluster).x;
                error_y += points.at(i).y - average.at(points.at(i).cluster).y;
                error_z += points.at(i).z - average.at(points.at(i).cluster).z;
                clustered_num++;
            }
        }
        error_x /= clustered_num;
        error_y /= clustered_num;
        error_z /= clustered_num;
        return sqrt(error_x * error_x + error_y * error_y + error_z * error_z);
    }
};