//
//  Data_Analysis.cpp
//  openCVTest
//
//  Created by Estelle on 02/03/2018.
//  Copyright © 2018 Estelle. All rights reserved.
//

#include "Data_Analysis.hpp"
#include <stdio.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/imgcodecs/imgcodecs.hpp"
using namespace cv;

#include <fstream>
#include <string>
#include <sstream>
#include "math.h"
#include <vector>
#define w 400
#include "Data_Analysis.hpp"

using namespace std;

/*
 * This cpp works with Transformation_to_2D.
 * By grouping the value to depth, we assign each pixel with different values
 * to represent depth value in the representation of RGB
 */


vector<double> Depth_Grouping(string path){
    vector<double> depth_LUT;
    vector<double> depth_grouping;
    string x_string;
    string y_string;
    string z_string;
    ifstream infile;
    infile.open(path, ios::in);
    while (!infile.eof()){
        infile >> x_string >> y_string >> z_string;
        //string stream(x_string);
        double y = stod(x_string);
        double z = stod(y_string);
        double x = stod(z_string);
        
        double depth =sqrt(x*x+y*y+z*z);
        depth_grouping.push_back(depth);
    }
   
    vector<double>::iterator max = max_element(begin(depth_grouping), end(depth_grouping));
    vector<double>::iterator min = min_element(begin(depth_grouping), end(depth_grouping));
    
    depth_LUT = depth_grouping;
    for (int i = 0; i<depth_grouping.size();i++){
        //depth_LUT[i] =round(255.0*(depth_grouping[i]- *min)/(*max - *min)+0.5);
        depth_LUT[i] =round(65535.0*(depth_grouping[i]- *min)/(*max - *min));
    }
    
    
    return depth_LUT;
}



