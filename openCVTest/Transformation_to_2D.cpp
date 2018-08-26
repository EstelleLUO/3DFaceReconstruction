//
//  Transformation_to_2D.cpp
//  openCVTest
//
//  Created by Estelle on 08/03/2018.
//  Copyright © 2018 Estelle. All rights reserved.
//


/*
 * This cpp includes codes to transfrom 3D cloud map to 2D depthmap
 */
#include "Transformation_to_2D.hpp"


int trans_2D(string path){
    // Create a new image
    int rows = 720;
    int cols = 1440;
    int number_pixel_radius = 4;
    Mat image(rows,cols,CV_16UC1,Scalar(0));
    
    string x_string;
    string y_string;
    string z_string;
    ifstream infile;
    const double pi = atan(1)*4;
    vector<double> depth_LUT;
    vector<double> depth_grouping;
    
    double x_map;
    double y_map;
    double depth;
    int i = 0;
    infile.open(path, ios::in);
    if (infile.fail()){
        cout << "error open!"<<endl;
    }
    depth_LUT = Depth_Grouping(path);
    int count = 0;
    while (!infile.eof()){
        count ++;
        infile >> x_string >> y_string >> z_string;
        double y = stod(x_string);
        double z = stod(y_string);
        double x = stod(z_string);
        // Divide the sphere into eight parts
        depth =sqrt(x*x+y*y+z*z);
        //int rows = image.rows;
        //int cols = image.cols;
        
        unsigned short int pixel = (unsigned short int)depth_LUT[i];
        
        if (x>0&&y>0&&z>=0){
            x_map = round(atan(y/x)*number_pixel_radius/pi*180)+cols/2;
            y_map = round(asin(sqrt(x*x+y*y)/depth)*number_pixel_radius/pi*180);
        }
        
        else if (x>0&&y==0&&z>=0)
        {
            x_map=cols/2;
            y_map =round(asin(sqrt(x*x+y*y)/depth)*number_pixel_radius/pi*180);
        }
        
        else if (x>0 && y<0&&z>=0){
            x_map = round(atan(x/(-y))*number_pixel_radius/pi*180)+cols/4;
            y_map = round(asin(sqrt(x*x+y*y)/depth)*number_pixel_radius/pi*180);
            
        }
        
        else if (x<0 && y>0&&z>=0){
            x_map = round(atan((-x)/y)*number_pixel_radius/pi*180)+3*cols/4;
            y_map = round(asin(sqrt(x*x+y*y)/depth)*number_pixel_radius/pi*180);
        }
        else if (x<0 && y==0&&z>=0){
            x_map = 3*cols/4;
            y_map = round(asin(sqrt(x*x+y*y)/depth)*number_pixel_radius/pi*180);
        }
            
        else if(x<0 && y<0&&z>=0){
            x_map = round(atan(y/x)*number_pixel_radius/pi*180);
            y_map = round(asin(sqrt(x*x+y*y)/depth)*number_pixel_radius/pi*180);
        }
        
        else if (x>0&&y>0&&z<0){
            x_map = round(atan(y/x)*number_pixel_radius/pi*180)+cols/2;
            y_map = rows- round(asin(sqrt(x*x+y*y)/depth)*number_pixel_radius/pi*180);
        }
        else if (x>0&&y==0&&z<0){
            x_map = cols/2;
            y_map = rows- round(asin(sqrt(x*x+y*y)/depth)*number_pixel_radius/pi*180);
        }

        else if (x>0 && y<0&&z<0){
            x_map =round(atan(x/(-y))*number_pixel_radius/pi*180)+cols/4;
            y_map =rows- round(asin(sqrt(x*x+y*y)/depth)*number_pixel_radius/pi*180);
        }
        
        else if (x<0 && y>0&&z<0){
            x_map = round(atan((-x)/y)*number_pixel_radius/pi*180)+3*cols/4;
            y_map = rows- round(asin(sqrt(x*x+y*y)/depth)*number_pixel_radius/pi*180);
        }
        else if (x<0 && y==0&&z<0){
            x_map = 3*cols/4;
            y_map = rows- round(asin(sqrt(x*x+y*y)/depth)*number_pixel_radius/pi*180);
        }
        else if (x<0 && y<0&&z<0){
            x_map = round(atan(y/x)*number_pixel_radius/pi*180);
            y_map = rows- round(asin(sqrt(x*x+y*y)/depth)*number_pixel_radius/pi*180);
        }
        
        else if (x==0 && y>=0&&z>=0){
            x_map = 3*cols/4;
            y_map = round(asin(sqrt(x*x+y*y)/depth)*number_pixel_radius/pi*180);
        }
        else if (x==0&&y<0&&z>=0){
            x_map = cols/4;
            y_map = round(asin(sqrt(x*x+y*y)/depth)*number_pixel_radius/pi*180);
        }
        else if (x==0 && y>=0&&z<0){
            x_map = 3*cols/4;
            y_map = round(asin(sqrt(x*x+y*y)/depth)*number_pixel_radius/pi*180)+rows/2;
        }
        else{
            x_map = cols/4;
            y_map = round(asin(sqrt(x*x+y*y)/depth)*number_pixel_radius/pi*180)+rows/2;
        }
        image.at<ushort>(y_map,x_map) = pixel;
    
        i++;
    }
    
    // Save Image
//    namedWindow("Display Window", WINDOW_AUTOSIZE);
//    imshow("Display Window", image);
    imwrite("/Users/estelle/Project/3DFaceReconstruction/gradient_test.png", image);
    //imwrite("/Users/estelle/Project/3DFaceReconstruction/depth_map.png", image);
    //waitKey(0);
    //destroyAllWindows();
    cout << count <<"origin"<<endl;
    return 0;
}

Mat image_cut(Mat src){
    int rows = src.rows;
    int cols = src.cols;
    cv::Mat temp = src(cv::Rect(0,0,rows,cols/2));
    cv::Mat temp2 =src(Range(0,rows),Range(cols/2,cols));
    string out_path= "/Users/estelle/Project/3DFaceReconstruction/temp1.png";
    string out_path2="/Users/estelle/Project/3DFaceReconstruction/temp2.png";
    imwrite(out_path, temp);
    imwrite(out_path2,temp2);
    Mat result;
    
    Mat mask = imread(out_path);
    Mat mask2 = imread(out_path2);
    hconcat(mask2,mask,result);
    
    return result;
}


