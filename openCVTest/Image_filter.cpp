//
//  Image_filter.cpp
//  openCVTest
//
//  Created by Estelle on 2018/6/27.
//  Copyright © 2018 Estelle. All rights reserved.
//

#include "Image_filter.hpp"
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
#include <vector>
#include "Reconstruction_of_mesh.hpp"
using namespace std;

Mat image_filter(Mat out_image, int filterSize, const double sigma){
    int rows = out_image.rows;
    int cols = out_image.cols;
    int index=65535; // The largest depth for each pixel
    Mat model_image;
    model_image = out_image.clone();
    
    // Gaussian Filter
    vector<vector<double>> gaus=GetGaussianKernel(filterSize, sigma);
    
    int sideDistance = (filterSize-1)/2;
    for (int i=sideDistance;i<rows-sideDistance;i++){
        for (int j=sideDistance;j<cols-sideDistance;j++){
            unsigned short int current =(unsigned short int)out_image.at<ushort>(i,j);
            if (current == 0){
                double sum = 0;
                double index_sum=0;
                int count = 0;
                for (int m=i-sideDistance;m<i+sideDistance+1;m++){
                    for (int n=j-sideDistance;n<j+sideDistance+1;n++){
                        unsigned short int temp =(unsigned short int) model_image.at<ushort>(m,n);
                        if (temp !=0){
                            int index_row = sideDistance-(i-m);
                            int index_col = sideDistance-(j-n);
                            sum += temp * gaus[index_row][index_col];
                            index_sum += gaus[index_row][index_col];
                            count++;
                        }
                    }
                }
                if (count != 0) sum = sum/index_sum;
                if (sum >index) sum=index;
                out_image.at<ushort>(i,j)= sum;
            }
        }
    }
    return out_image;
}

Mat full_image_filter(Mat out_image, int filterSize, const double sigma){
    int rows = out_image.rows;
    int cols = out_image.cols;
    int index=65535; // The largest depth for each pixel
    Mat model_image;
    model_image = out_image.clone();
    
    // Gaussian Filter
    vector<vector<double>> gaus=GetGaussianKernel(filterSize, sigma);
    
    int sideDistance = (filterSize-1)/2;
    for (int i=sideDistance;i<rows-sideDistance;i++){
        for (int j=sideDistance;j<cols-sideDistance;j++){
                double sum = 0;
                double index_sum=0;
                int count = 0;
                for (int m=i-sideDistance;m<i+sideDistance+1;m++){
                    for (int n=j-sideDistance;n<j+sideDistance+1;n++){
                        unsigned short int temp =(unsigned short int) model_image.at<ushort>(m,n);
                        int index_row = sideDistance-(i-m);
                        int index_col = sideDistance-(j-n);
                        sum += temp * gaus[index_row][index_col];
                        index_sum += gaus[index_row][index_col];
                        count++;
                    }
                }
                if (count != 0) sum = sum/index_sum;
                if (sum >index) sum=index;
                out_image.at<ushort>(i,j)= sum;
        }
    }
    return out_image;
}

/*************************************************************************************
 * The function to obtain the guassian kernel
 * The parameter gaus: the vector stores the gauss filter template
 * The parameter size: the size of the gaussian kernel
 * The paramter sigma: the standard deviation of the gaussian kernel
 *************************************************************************************/
 
vector<vector<double>> GetGaussianKernel(const int size, const double sigma){
    double temp[size];
    temp[size]={0.0};
    vector<vector<double>> gaus;
    vector<double> gausCoefficient(temp,temp+size);
    for (int i=0;i<size;i++) gaus.push_back(gausCoefficient);
    
    const double PI=4.0*atan(1.0);
    int center=size/2;
    double sum=0;
    for (int i=0;i<size;i++){
        for (int j=0;j<size;j++){
            gaus[i][j]=(1/(2*PI*sigma*sigma))*exp(-((i-center)*(i-center)+(j-center)*(j-center))/(2*sigma*sigma));
            sum+=gaus[i][j];
        }
    }
    for(int i=0;i<size;i++){
        for (int j=0;j<size;j++){
            gaus[i][j]/=sum;
        }
    }
    return gaus;
}


