//
//  Reconstruction_of_mesh.cpp
//  openCVTest
//
//  Created by Estelle on 2018/6/16.
//  Copyright © 2018 Estelle. All rights reserved.
//

#include "Reconstruction_of_mesh.hpp"

/***********************************************************************************************************
 * Mesh Reconstruction: Take the nearest right point and the nearest right point on the next line
 ************************************************************************************************************/
int recons_Mesh(vector<vector<int> >position){
    string path = "/Users/estelle/Documents/faceFitResult/2D_Transformation/depth_map.png";
    Mat image = imread(path,CV_LOAD_IMAGE_UNCHANGED);
    ofstream outMeshInfo("/Users/estelle/Documents/faceFitResult/2D_Transformation/mesh.txt",ios::trunc);
    
    int rows = image.rows;
    int cols = image.cols;
    // The last row does not need to go through the process of remesh
    for (int i = 0; i<rows-1;i++){
        for (int j=0;j<cols;j++){
            unsigned short int current = (unsigned short int)image.at<ushort>(i,j);
            if (current != 0){
                // Fine the nearest right point on the same row
                int cursor_i_col;
                int cursor_j_col;
                if (j==cols-1){
                    cursor_i_col=i;
                    cursor_j_col=0;
                }
                else{
                    cursor_i_col = i;
                    cursor_j_col = j+1;
                }
                unsigned short int next_col = (unsigned short int)image.at<ushort>(cursor_i_col,cursor_j_col);
                while (next_col == 0){
                    cursor_j_col++;
                    if (cursor_j_col == cols) cursor_j_col =0;
                    next_col = (unsigned short int)image.at<ushort>(cursor_i_col,cursor_j_col);
                }
                // Find the nearest right point on the next row
                int cursor_i_row = i+1;
                int cursor_j_row = j;
                unsigned short int next_row = (unsigned short int)image.at<ushort>(cursor_i_row,cursor_j_row);
                // Check if the next row is all black
                int flag = cursor_j_row;
                while (next_row == 0){
                  cursor_j_row++;
                  if (cursor_j_row == cols) cursor_j_row =0;
                  if (cursor_j_row == flag) break;
                  next_row = (unsigned short int)image.at<ushort>(cursor_i_row,cursor_j_row);
                }
                
                int mark1=-1; // Center point
                int mark2=-1; // The point next to the center point
                int mark3=-1; // The point below the center point
                int mark4=-1; // The point below mark 2
                mark1 = position[i][j];
                mark2 = position[cursor_i_col][cursor_j_col];
                mark3 = position[cursor_i_row][cursor_j_row];
                mark4 = position[cursor_i_col+1][cursor_j_col];
                
                if (mark1 != mark2 && mark2 != mark3 && mark1 != mark3 && mark1!=0 &&mark2!=0 &&mark3!=0){
                    outMeshInfo << "f " << mark1 << "/" <<  mark1 << "/" << mark1 << " " << mark3 << "/" <<  mark3 << "/" << mark3 << " " <<mark2 << "/" <<  mark2 << "/" << mark2 <<endl;
                    outMeshInfo << "f " << mark2 << "/" <<  mark2 << "/" << mark2 << " " << mark3 << "/" <<  mark3 << "/" << mark3 << " " <<mark4 << "/" <<  mark4 << "/" << mark4 <<endl;
                }

                /*
                int size = position.size();
                int mark1=-1;
                int mark2=-1;
                int mark3=-1;
                for (int index=0;index<size;index++){
                    if (position[index][0]==i && position[index][1]==j) mark1 = index;
                    if (position[index][0]==cursor_i_col && position[index][1]==cursor_j_col) mark2 = index;
                    if (position[index][0]==cursor_i_row && position[index][1]==cursor_j_row) mark3 = index;
                }
                cout << mark1 << "mark1"<<endl;
                cout << mark2 << "mark2" <<endl;
                cout << mark3 << "mark3" <<endl;
                if (mark1 != -1 && mark2 !=-1 && mark3 != -1){
                    outMeshInfo << "f " << mark1 << "/" <<  mark1 << "/" << mark1 << " " << mark2 << "/" <<  mark2 << "/" << mark2 << " " <<mark3 << "/" <<  mark3 << "/" << mark3 <<endl;
                }
                */
            }
        }
    }
    return 0;
}
