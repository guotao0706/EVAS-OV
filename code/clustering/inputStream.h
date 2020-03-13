#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <io.h>
#include "point.h"

using namespace std;

//read the viewport file(x, y, z)
class InputReader {
private:
    ifstream fin;
    vector<vector<Point>> points;
    vector<string> folders;
public:
    double average_dis = 0;
    double max_dis = 0;
    double min_dis = 100;
    int point_num = 0;
    int folder_num = 0;

    /***
    read viewport files that contain viewport data(x, y, z) and divided them into vector<vector<Point>> according to users
    :param path: path that contains all users viewport file
    :param filename: name of the video's viewport file
    :param num: number of the viewport data to read in each user's file
    ***/
    InputReader(string path, string filename, int num) {
        folders = vector<string>();
        points = vector<vector<Point>>();
        point_num = num;
        long hFile = 0;
        struct _finddata_t fileInfo;    //file info

        //If empty folder
        if ((hFile = _findfirst((path + "\\*").c_str(), &fileInfo)) == -1) {
            return;
        }

        //Iterate through all folders that contains viewport data
        do
        {
            folder_num++;
            if (folder_num <= 2) continue;  //remove "." and ".."
            //cout << fileInfo.name << (fileInfo.attrib & _A_SUBDIR ? "[folder]" : "[file]") <<endl;
            folders.push_back(path + "\\" + fileInfo.name);
            //cout << path + "\\" + fileInfo.name << endl;
            vector<Point> newFile(point_num);
            string newFilePath = folders.back() + "\\" +filename;   //one user's viewport file's path
            //cout << newFilePath << endl;

            fin.open(newFilePath);
            //read the viewport data
            for (int i = 0; i < point_num; i++) {
              
                double x, y, z, vx = 0, vy = 0, vz = 0;
                double r1, r2;
                fin >> r1 >> r2 >> x >> y >> z;
                if (i == 0) vx = 0, vy = 0, vz = 0;
                else {
                    //Speed(vx, vy, vz) is calculated as the difference between the viewpoint position 
                    //at the current moment and the viewpoint position at the previous moment and then 
                    //divided by 2 to control the range to be the same as the range of postion(-1 -> 1)
                    vx = x - newFile.at(i - 1).x;
                    vy = y - newFile.at(i - 1).y;
                    vz = z - newFile.at(i - 1).z;
                    vx /= 2;
                    vy /= 2;
                    vz /= 2;
                }
                //Point newp(x, y, z, vx, vy, vz, 0, NOT_CLASSIFIED, 0.5);

                //saved as a new point
                newFile.at(i) = { x, y, z, vx, vy, vz, 0, NOT_CLASSIFIED, 0.8};
            }
            points.push_back(newFile);

            fin.close();
        } while (_findnext(hFile, &fileInfo) == 0);
        folder_num -= 2;    //remove "." and ".."
        _findclose(hFile);
        return;    
    }

    vector<vector<Point>> getPoints() {
        return points;
    }
};