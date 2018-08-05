//
//  Reconstruction_to_3D.cpp
//  openCVTest
//
//  Created by Estelle on 09/03/2018.
//  Copyright © 2018 Estelle. All rights reserved.
//

#include "Reconstruction_to_3D.hpp"
#include "image_info.hpp"
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
#include <vector>
#include "Reconstruction_of_mesh.hpp"
#include "Image_filter.hpp"
#include "Resolution.hpp"
using namespace std;

/*
 * This file can read data from 2D depth map and then output a txt file
 * with data of each pixels in it
 */
int recons_3D(string path){
    // Recons_3D read into depth_map.bmp and carries out image processing on it
    // Then it outputs an new_depth_map which is new result_image
    Mat image = imread(path,CV_LOAD_IMAGE_UNCHANGED);
   
    Mat out_image;
    out_image = image.clone();
    
    const double pi = atan(1)*4;
    int rows = out_image.rows;
    int cols = out_image.cols;
    
/***************************************************************************************
 * Reconstruct the sparse points to be more densed ones
 */
    int origin = 0;
    for (int i=1;i<rows-1;i++){
        for (int j=1;j<cols-1;j++){
            //if (flag ==1) continue;
            unsigned short int current =(unsigned short int)out_image.at<ushort>(i,j);
            //unsigned char next =out_image.at<Vec3b>(i,j+1)[0];
            //flag=0;
            //if (next!=0) flag=1;
            if (current!=0){
                origin++;
                out_image.at<ushort>(i,j-1)=current;


                out_image.at<ushort>(i-1,j-1)=current;

                out_image.at<ushort>(i-1,j)=current;
            }
        }
    }
    cout << origin << endl;
//
//    change_resolution(out_image);

//    Mat temp_img = imread("/Users/estelle/Documents/faceFitResult/2D_Transformation/resolution_map.png",CV_LOAD_IMAGE_UNCHANGED);
//    change_resolution(temp_img);
    //Mat img = imread("/Users/estelle/Documents/faceFitResult/2D_Transformation/depth_map.png",CV_LOAD_IMAGE_UNCHANGED);
    //Mat img = out_image.clone();
//
//    int new_rows = img.rows;
//    int new_cols = img.cols;
//    for (int i = 0; i<new_rows;i++){
//        int startPoint=-1;  //depth info
//        int endPoint = -1;
//        int startcols = -1;  //position
//        int endcols = -1;
//
//        for (int j = 0;j <new_cols;j++){
//            unsigned short int start = (unsigned short int)img.at<ushort>(i,j);
//            if(start != 0){
//                startPoint = start;
//                startcols = j;
//                for (int x=new_cols-1;x>=j;x--){
//                    unsigned short int end = (unsigned short int) img.at<ushort>(i,x);
//                    if (end != 0){
//                        endPoint = end;
//                        endcols = x;
//                        break;
//                    }
//                }
//                break;
//            }
//        }
//        double distToOrigin = 7;
//        int step = startcols+new_cols-endcols;
//        double step_size =(double) (endPoint-startPoint)/step;
//        if (startcols == endcols || startcols == -1) continue;
//        else{
//            for (int m=0;m<=startcols;m++){
//                int distance = startcols-m;
//                double step_index=distance*step_size + startPoint;
//                double radius = sqrt(step_index*step_index+distToOrigin*distToOrigin-2*distToOrigin*step_index*cos((180-m)/pi));
//                img.at<ushort>(i,m) = radius;
//            }
//
//            for (int n=new_cols-1;n>=endcols;n--){
//                int distance = startcols+new_cols-n;
//                double step_index=distance*step_size + startPoint;
//                double radius = sqrt(step_index*step_index+distToOrigin*distToOrigin-2*distToOrigin*step_index*cos((180-n)/pi));
//                img.at<ushort>(i,n) = radius;
//            }
//        }
//    }

    
    // Image Filter

    //img = image_filter(img,5,1);
//    img = image_filter(img,5,2);
//    img = image_filter(img,5,2);
      out_image = image_filter(out_image,5,1);
      out_image = image_filter(out_image,5,1);
      //out_image = full_image_filter(out_image,5,1);
//    Mat src = img.clone();
//    GaussianBlur(src, img, Size(3,3), 1);
    
    namedWindow("Display Window", WINDOW_AUTOSIZE);
    imshow("Display Window", out_image);
    imwrite(path, out_image);
    
    trans_3D(path);
    return 0;
}

void trans_3D(string path){
    // Get min and Max for further transformation
    
    Mat image = imread(path,CV_LOAD_IMAGE_UNCHANGED);
    int rows = image.rows;
    int cols = image.cols;
    
    const double pi = atan(1)*4;
    double x_map;
    double y_map;
    //unsigned char depth;
    double depth;
    double x_3d;
    double y_3d;
    double z_3d; // These three coordinates are in Mashlabs form, which is different from normal norm.
    
    vector<double> depth_grouping;
    string x_string;
    string y_string;
    string z_string;
    
    // Get the max depth and the min depth
    ifstream infile;
    //string original_path = "/Users/estelle/Documents/faceFitResult/2D_Transformation/resolution_reference.txt";
    string original_path = "/Users/estelle/Documents/faceFitResult/2D_Transformation/merge3d_John.txt";
    infile.open(original_path, ios::in);
    while (!infile.eof()){
        infile >> x_string >> y_string >> z_string;
        string stream(x_string);
        double y = stod(x_string);
        double z = stod(y_string);
        double x = stod(z_string);
        
        double depth =sqrt(x*x+y*y+z*z);
        depth_grouping.push_back(depth);
    }
    
    vector<double>::iterator max = max_element(begin(depth_grouping), end(depth_grouping));
    vector<double>::iterator min = min_element(begin(depth_grouping), end(depth_grouping));
    
    double max_double = *max;
    double min_double = *min;
    cout << max_double<<"original max"<<endl;
    cout << min_double<<"original min"<<endl;
 
    ofstream outfile("/Users/estelle/Documents/faceFitResult/2D_Transformation/new_result.txt",ios::trunc);
    
    // Record the row number that the point is in
    //vector<vector<int>> position;
    int temp[720] = {0};
    vector<int> position_col(temp,temp+cols);
    vector<vector<int> > position;
    for (int i = 0;i<rows;i++){
        position.push_back(position_col);
    }
    // Record the row number of each point in the txt file
    int count = 0;
    int index = 65536; // Mark the upper bound of each pixel
    for (int i=cols/2;i<(3*cols/4);i++)
    {
        x_map = i;
        for (int j = 0;j<(rows/2);j++)
        {
            y_map = j;
            depth = (unsigned short int)image.at<ushort>(y_map,x_map);
            if (depth == 0) continue;
            depth = depth*(max_double - min_double)/index+min_double;
            y_3d = depth*cos(y_map*pi/rows);
            x_3d = depth*sin(y_map*pi/rows)*sin((x_map-cols/2)*pi/rows);
            z_3d = depth*sin(y_map*pi/rows)*cos((x_map-cols/2)*pi/rows);
            outfile<<"v "  << x_3d <<" " << y_3d <<" "<< z_3d <<endl;
            //outfile<<x_3d <<" " << y_3d <<" "<< z_3d <<endl;
            count++;
            position[j][i] = count;
            //int temp[2] = {j,i};
            //vector<int> eachPosition(temp,temp+2);
            //position.push_back(eachPosition);
        }
        for (int j=rows/2;j<rows;j++)
        {
            y_map=j;
            depth = (unsigned short int)image.at<ushort>(y_map,x_map);
            if (depth == 0) continue;
            depth = depth*(max_double - min_double)/index+min_double;
            y_3d = - depth*cos((rows-y_map)*pi/rows);
            x_3d = depth*sin((rows-y_map)*pi/rows)*sin((x_map-cols/2)*pi/rows);
            z_3d = depth*sin((rows-y_map)*pi/rows)*cos((x_map-cols/2)*pi/rows);
            outfile <<"v " << x_3d <<" " << y_3d <<" "<< z_3d <<endl;
            //outfile<<x_3d <<" " << y_3d <<" "<< z_3d <<endl;
            count++;
            position[j][i] = count;
            //int temp[2] = {j,i};
//            vector<int> eachPosition(temp,temp+2);
//            position.push_back(eachPosition);
        }
    }
    
    for (int i=3*cols/4;i<cols;i++)
    {
        x_map = i;
        for (int j = 0;j<(rows/2);j++)
        {
            y_map = j;
            depth = (unsigned short int)image.at<ushort>(y_map,x_map);
            if (depth == 0) continue;
            depth = depth*(max_double - min_double)/index+min_double;
            y_3d = depth*cos(y_map*pi/rows);
            z_3d = - depth*sin(y_map*pi/rows)*sin((x_map-3*cols/4)*pi/rows);
            x_3d = depth*sin(y_map*pi/rows)*cos((x_map-3*cols/4)*pi/rows);
            outfile<<"v "  << x_3d <<" " << y_3d <<" "<< z_3d <<endl;
            //outfile<<x_3d <<" " << y_3d <<" "<< z_3d <<endl;
            count++;
            position[j][i] = count;
//            int temp[2] = {j,i};
//            vector<int> eachPosition(temp,temp+2);
//            position.push_back(eachPosition);
        }
        for (int j=rows/2;j<rows;j++)
        {
            y_map=j;
            depth = (unsigned short int)image.at<ushort>(y_map,x_map);
            if (depth == 0) continue;
            depth = depth*(max_double - min_double)/index+min_double;
            y_3d = - depth*cos((rows-y_map)*pi/rows);
            z_3d = - depth*sin((rows-y_map)*pi/rows)*sin((x_map-3*cols/4)*pi/rows);
            x_3d = depth*sin((rows-y_map)*pi/rows)*cos((x_map-3*cols/4)*pi/rows);
            outfile<<"v "  << x_3d <<" " << y_3d <<" "<< z_3d <<endl;
            //outfile<<x_3d <<" " << y_3d <<" "<< z_3d <<endl;
            count++;
            position[j][i] = count;
//            int temp[2] = {j,i};
//            vector<int> eachPosition(temp,temp+2);
//            position.push_back(eachPosition);
        }
    }
    
    for (int i=0;i<(cols/4);i++)
    {
        x_map = i;
        for (int j = 0;j<(rows/2);j++)
        {
            y_map = j;
            depth = (unsigned short int)image.at<ushort>(y_map,x_map);
            if (depth == 0) continue;
            depth = depth*(max_double - min_double)/index+min_double;
            y_3d = depth*cos((y_map)*pi/rows);
            x_3d = - depth*sin((y_map)*pi/rows)*sin(x_map*pi/rows);
            z_3d = - depth*sin((y_map)*pi/rows)*cos(x_map*pi/rows);
            outfile<<"v "  << x_3d <<" " << y_3d <<" "<< z_3d <<endl;
            //outfile<<x_3d <<" " << y_3d <<" "<< z_3d <<endl;
            count++;
            position[j][i] = count;
//            int temp[2] = {j,i};
//            vector<int> eachPosition(temp,temp+2);
//            position.push_back(eachPosition);
        }
        for (int j=rows/2;j<rows;j++)
        {
            y_map=j;
            depth = (unsigned short int)image.at<ushort>(y_map,x_map);
            if (depth == 0) continue;
            depth = depth*(max_double - min_double)/index+min_double;
            y_3d = - depth*cos((rows-y_map)*pi/rows);
            x_3d = - depth*sin((rows-y_map)*pi/rows)*sin(x_map*pi/rows);
            z_3d = - depth*sin((rows-y_map)*pi/rows)*cos(x_map*pi/rows);
            outfile<<"v "  << x_3d <<" " << y_3d <<" "<< z_3d <<endl;
            //outfile<<x_3d <<" " << y_3d <<" "<< z_3d <<endl;
            count++;
            position[j][i] = count;
//            int temp[2] = {j,i};
//            vector<int> eachPosition(temp,temp+2);
//            position.push_back(eachPosition);
        }
    }
    
    for (int i=cols/4;i<(cols/2);i++)
    {
        x_map = i;
        for (int j = 0;j<(rows/2);j++)
        {
            y_map = j;
            depth = (unsigned short int)image.at<ushort>(y_map,x_map);
            if (depth == 0) continue;
            depth = depth*(max_double - min_double)/index+min_double;
            y_3d = depth*cos(y_map*pi/rows);
            z_3d = depth*sin(y_map*pi/rows)*sin((x_map-cols/4)*pi/rows);
            x_3d = - depth*sin(y_map*pi/rows)*cos((x_map-cols/4)*pi/rows);
            outfile<<"v " << x_3d <<" " << y_3d <<" "<< z_3d <<endl;
            //outfile<<x_3d <<" " << y_3d <<" "<< z_3d <<endl;
            count++;
            position[j][i] = count;
//            int temp[2] = {j,i};
//            vector<int> eachPosition(temp,temp+2);
//            position.push_back(eachPosition);
            
        }
        for (int j=rows/2;j<rows;j++)
        {
            y_map=j;
            depth = (unsigned short int)image.at<ushort>(y_map,x_map);
            if (depth == 0) continue;
            depth = depth*(max_double - min_double)/index+min_double;
            y_3d = -depth*cos((rows-y_map)*pi/rows);
            z_3d = depth*sin((rows-y_map)*pi/rows)*sin((x_map-cols/4)*pi/rows);
            x_3d = - depth*sin((rows-y_map)*pi/rows)*cos((x_map-cols/4)*pi/rows);
            outfile<<"v " << x_3d <<" " << y_3d <<" "<< z_3d <<endl;
            //outfile<<x_3d <<" " << y_3d <<" "<< z_3d <<endl;
            count++;
            position[j][i] = count;
//            int temp[2] = {j,i};
//            vector<int> eachPosition(temp,temp+2);
//            position.push_back(eachPosition);
        }
    }
    recons_Mesh(position);
    return;
}
