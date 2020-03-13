#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

//output the index of users that belong to one cluster into txt files
class OutputPrinter {
private:
    ofstream fout;
    vector<vector<int> > cluster;
    string filename;
    int n;
public:
    /***
    output the index of users that belong to one cluster into txt files
    :param n: number of users(not used)
    :param filename: path of the txt file that need to be saved
    :param cluster: vector of groups that contains the index of users belonging to one group
    ***/
    OutputPrinter(int n, string filename, vector<vector<int>> cluster) {
        this->n = n;
        this->cluster = cluster;

        // remove ".txt" from filename
        if (filename.size() < 4) {
            cout << filename << "input file name's format is wrong\n";
            exit(0);
        }
        for (int i = 0; i < 4; i++) filename.pop_back();
        this->filename = filename;
    }
    void print() {
        for (int i = 0; i < n; i++) {
            fout.open(filename + "_cluster_" + to_string(i) + ".txt");

            for (int j = 0; j < (int)(cluster[i].size()); j++) {
                fout << cluster[i][j] << endl;
            }

            fout.close();
        }
    }
};