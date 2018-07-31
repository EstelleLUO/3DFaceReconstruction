//
//  CutandMerge.cpp
//  openCVTest
//
//  Created by Estelle on 2018/6/28.
//  Copyright © 2018 Estelle. All rights reserved.
//

#include "CutandMerge.hpp"
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
#include <vector>
#include "Reconstruction_of_mesh.hpp"
#include "Image_filter.hpp"
using namespace std;

Mat img_merge(Mat image1, Mat image2){
    Mat imgMerge;
    Size size(image1.cols+image2.cols,MAX(image1.rows,image1.rows));
    imgMerge.create(size,CV_MAKETYPE(image1.depth(),3));
    imgMerge = Scalar::all(0);
    Mat outImgLeft, outImgRight;
    outImgLeft = imgMerge(Rect(0,0,image1.cols,image1.rows));
    outImgRight = imgMerge(Rect(image1.cols,0,image1.cols,image1.rows));
    image1.copyTo(outImgLeft);
    image2.copyTo(outImgRight);
    namedWindow("Test",0);
    imshow("Test",imgMerge);
    waitKey(0);
    return imgMerge;
}
