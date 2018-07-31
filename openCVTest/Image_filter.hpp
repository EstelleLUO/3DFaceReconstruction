//
//  Image_filter.hpp
//  openCVTest
//
//  Created by Estelle on 2018/6/27.
//  Copyright © 2018 Estelle. All rights reserved.
//

#ifndef Image_filter_hpp
#define Image_filter_hpp

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

Mat image_filter(Mat out_image, int filterSize, const double sigma);
Mat full_image_filter(Mat out_image, int filterSize, const double sigma);
vector<vector<double>> GetGaussianKernel(const int size, const double sigma);

#endif /* Image_filter_hpp */
