#include "inputStream.h"
#include "point.h"
#include "outputStream.h"
#include "DBSCAN.h"

int main(int argc, const char* argv[]) {
	string viewdata_folder = "../../database/viewpoint_xyz";
	string viewdata_file = "Venise-s-AJRFQuAtE_0.txt";
	int viewdata_num = 1800;
    float w = 0.8;
    int minPts = 5;
    double eps = 0.2;

    string cluster_output = "../Venise.txt";
    string MSE_file = "../Venise_1800_0.2_5_w0.8.txt";

    InputReader inputReader(viewdata_folder, viewdata_file, viewdata_num, w);

    ofstream outcome;

    outcome.open(MSE_file);
    for (int i = 0; i < inputReader.point_num; i++) {
        vector<Point> p(inputReader.folder_num);
        for (int j = 0; j < inputReader.folder_num; j++) {
            p.at(j) = inputReader.getPoints().at(j).at(i);
        }
        DBSCAN dbScan(inputReader.folder_num, eps, minPts, p);
		if (i == 0) {
			//output the clustering results of users' viewpoints at the first moment for demo
            OutputPrinter cluster_demo(58, cluster_output, dbScan.getCluster);
		}
        dbScan.run();
        //cout << i << endl;
        outcome << dbScan.getMSE() << endl;
    }
    outcome.close();
}