//
//  main.cpp
//  openCVTest
//
//  Created by Estelle on 13/01/2018.
//  Copyright © 2018 Estelle. All rights reserved.
//

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
#define w 400
#include "Data_Analysis.hpp"
#include "Reconstruction_to_3D.hpp"
#include "Transformation_to_2D.hpp"
#include "image_info.hpp"
#include "Reconstruction_of_mesh.hpp"
#include "Image_filter.hpp"
#include "Image_filter.hpp"
#include "CutandMerge.hpp"
using namespace std;

//int main() {
//    string path = "/Users/estelle/Documents/faceFitResult/2D_Transformation/merge3d_John.txt";
//    string out_path ="/Users/estelle/Documents/faceFitResult/2D_Transformation/depth_map.png";
//    trans_2D(path);
//    //recons_3D(out_path);
//    return 0;
//}

int main() {
    string path = "/Users/estelle/Project/3DFaceReconstruction/merge3d_John.txt";
    string out_path ="/Users/estelle/Project/3DFaceReconstruction/gradient_test.png";
    string result_path ="/Users/estelle/Project/3DFaceReconstruction/gradient_test_result.png";
    trans_2D(path);
    recons_3D(out_path);
    Mat src = imread(out_path);
    src = image_cut(src);
    imwrite(result_path, src);

    Mat grad_x, grad_y;
    Mat abs_grad_x,abs_grad_y;
    Mat grad;
    int scale = 1;
    int delta = 0;
    int ddepth = CV_16U;


    Scharr( src, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT );
    convertScaleAbs( grad_x, abs_grad_x );

    Scharr( src, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );
    convertScaleAbs( grad_y, abs_grad_y );

    addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad );

    imshow("grad window", grad );
    
    waitKey(0);


    return 0;
}

/*
int main(){
    string x_ori;
    string y_ori;
    string z_ori;
    x_ori = "1.7018";
    y_ori = "-5.6369";
    z_ori = "-4.6918";
    double max = 16.2828;
    double min = 4.2828;
    const double pi = atan(1)*4;
    double y = stod(x_ori);
    double z = stod(y_ori);
    double x = stod(z_ori);
    double depth = sqrt(x*x+ y*y +z*z);
    
    int x_map = round(atan((-x)/y)*2/pi*180)+540;
    int y_map = 360- round(asin(sqrt(x*x+y*y)/depth)*2/pi*180);
    depth = round(255.0*(depth- min)/(max - min));
    depth = depth*(max - min)/255+min;
    
    double x_3d;
    double y_3d;
    double z_3d;
    y_3d = - depth*cos((360-y_map)*pi/360);
    z_3d = - depth*sin((360-y_map)*pi/360)*sin((x_map-540)*pi/360);
    x_3d = depth*sin((360-y_map)*pi/360)*cos((x_map-540)*pi/360);
    
    //depth = depth*(max - min)/255+min;
    //y_3d = -sqrt(depth*depth - x_3d*x_3d - z_3d*z_3d);
    //cout << depth<<endl;
    cout << x_3d<<endl;
    cout << y_3d<<endl;
    cout << z_3d<<endl;
    return 0;
}
*/

/****************************************************************************************
 * Use 10*10 sphere to test
 
int main(){
    int rows=360;
    int cols=720;
    Mat image(rows,cols,CV_8UC3,cv::Scalar::all(0));
    for (int i=0;i<rows;i++){
        for (int j=0;j<cols;j++){
            image.at<Vec3b>(i,j)[0]=150;
            image.at<Vec3b>(i,j)[1]=150;
            image.at<Vec3b>(i,j)[2]=150;
        }
    }
    string path = "/Users/estelle/Documents/faceFitResult/2D_Transformation/test_depth_map.bmp";
    namedWindow("Display Window",WINDOW_AUTOSIZE);
    imshow("Display Window",image);
    //waitKey(0);
    imwrite(path, image);
    
    recons_3D(path);
    return 0;
}
 *****************************************************************************************/

