#include "inputStream.h"
#include "point.h"
#include "outputStream.h"
#include "DBSCAN.h"

int main(int argc, const char* argv[]) {
	string viewdata_folder = "../../database/viewpoint_xyz";
	string viewdata_file = "Venise-s-AJRFQuAtE_0.txt";
	int viewdata_num = 1800;
    string cluster_output = "../Venise.txt";
    InputReader inputReader(viewdata_folder, viewdata_file, viewdata_num);

    ofstream outcome;

    outcome.open("../Timelapse_1800_0.2_5_w0.8.txt");
    for (int i = 0; i < inputReader.point_num; i++) {
        vector<Point> p(inputReader.folder_num);
        for (int j = 0; j < inputReader.folder_num; j++) {
            p.at(j) = inputReader.getPoints().at(j).at(i);
        }
        DBSCAN dbScan(inputReader.folder_num, 0.2, 5, p);
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