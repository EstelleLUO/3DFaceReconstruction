//
//  Resolution.cpp
//  openCVTest
//
//  Created by Estelle on 2018/7/8.
//  Copyright © 2018 Estelle. All rights reserved.
//

#include "Resolution.hpp"
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
#include <vector>
using namespace std;
int change_resolution(Mat image){
    int cols = image.cols;
    int rows = image.rows;
    int new_cols = cols/2;
    int new_rows = rows/2;
    Mat img(new_rows,new_cols,CV_16UC1,1);
    for (int i=0; i<rows;i=i+2){
        for (int j=0;j<cols;j=j+2){
            unsigned short int current = (unsigned short int)image.at<ushort>(i,j);
            unsigned short int next_col =(unsigned short int)image.at<ushort>(i+1,j);
            unsigned short int next_row =(unsigned short int)image.at<ushort>(i,j+1);
            unsigned short int next_col_row=(unsigned short int)image.at<ushort>(i+1,j+1);
            int sum = 0;
            int count=0;
            if(current!=0){
                sum = sum+current;
                count++;
            }
            if(next_col!=0){
                sum = sum+next_col;
                count++;
            }
            if(next_row!=0){
                sum = sum+next_row;
                count++;
            }
            if(next_col_row!=0){
                sum = sum+next_col_row;
                count++;
            }
            int out_rows = i/2;
            int out_cols = j/2;
            if (count!=0){
                int result = sum/count;
                img.at<ushort>(out_rows,out_cols) = result;
            }
            else{
                img.at<ushort>(out_rows,out_cols)= 0;
            }
        }
    }
    string out_path = "/Users/estelle/Documents/faceFitResult/2D_Transformation/resolution_map.png";
    imwrite(out_path,img);
    return 0;
}

