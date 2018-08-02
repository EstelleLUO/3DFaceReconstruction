//
//  Transformation_to_2D.hpp
//  openCVTest
//
//  Created by Estelle on 08/03/2018.
//  Copyright © 2018 Estelle. All rights reserved.
//

#ifndef Transformation_to_2D_hpp
#define Transformation_to_2D_hpp
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
#include <stdio.h>
#include "math.h"
#include "Data_Analysis.hpp"
#include "Reconstruction_to_3D.hpp"
#include <stdio.h>
using namespace std;

int trans_2D(string path);
Mat image_cut(Mat src);

#endif /* Transformation_to_2D_hpp */
