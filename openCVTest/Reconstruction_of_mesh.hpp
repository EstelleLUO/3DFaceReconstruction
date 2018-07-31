//
//  Reconstruction_of_mesh.hpp
//  openCVTest
//
//  Created by Estelle on 2018/6/16.
//  Copyright © 2018 Estelle. All rights reserved.
//

#ifndef Reconstruction_of_mesh_hpp
#define Reconstruction_of_mesh_hpp

#include <stdio.h>
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
int recons_Mesh(vector<vector<int> >position);

#endif /* Reconstruction_of_mesh_hpp */
