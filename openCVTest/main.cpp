#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"

#include <iostream>
#include <string>

#include "utils.hpp"
using namespace std;
using namespace cv;

/*
 * Note: This program uses C assert() statements, define NDEBUG marco to
 * disable assertions.
 */

/* Inpainting*/
#ifndef DEBUG
#define DEBUG 0
#endif

int main (int argc, char** argv) {
    // --------------- read filename strings ------------------
    std::string colorFilename, maskFilename;
    
    if (argc == 3) {
        colorFilename = argv[1];
        maskFilename = argv[2];
    } else {
        std::cerr << "Usage: ./inpainting colorImageFile maskImageFile" << std::endl;
        return -1;
    }
    
    // ---------------- read the images ------------------------
    // colorMat     - color picture + border
    // maskMat      - mask picture + border
    // grayMat      - gray picture + border
    cv::Mat colorMat, maskMat, grayMat;
    loadInpaintingImages(
                         colorFilename,
                         maskFilename,
                         colorMat,
                         maskMat,
                         grayMat
                         );
    
    // confidenceMat - confidence picture + border
    cv::Mat confidenceMat;
    maskMat.convertTo(confidenceMat, CV_32F);
    confidenceMat /= 255.0f;
    
    // add borders around maskMat and confidenceMat
    cv::copyMakeBorder(maskMat, maskMat,
                       RADIUS, RADIUS, RADIUS, RADIUS,
                       cv::BORDER_CONSTANT, 255);
    cv::copyMakeBorder(confidenceMat, confidenceMat,
                       RADIUS, RADIUS, RADIUS, RADIUS,
                       cv::BORDER_CONSTANT, 0.0001f);
    
    // ---------------- start the algorithm -----------------
    
    contours_t contours;            // mask contours
    hierarchy_t hierarchy;          // contours hierarchy
    
    
    // priorityMat - priority values for all contour points + border
    cv::Mat priorityMat(
                        confidenceMat.size(),
                        CV_32FC1
                        );  // priority value matrix for each contour point
    
    assert(
           colorMat.size() == grayMat.size() &&
           colorMat.size() == confidenceMat.size() &&
           colorMat.size() == maskMat.size()
           );
    
    cv::Point psiHatP;          // psiHatP - point of highest confidence
    
    cv::Mat psiHatPColor;       // color patch around psiHatP
    
    cv::Mat psiHatPConfidence;  // confidence patch around psiHatP
    double confidence;          // confidence of psiHatPConfidence
    
    cv::Point psiHatQ;          // psiHatQ - point of closest patch
    
    cv::Mat result;             // holds result from template matching
    cv::Mat erodedMask;         // eroded mask
    
    cv::Mat templateMask;       // mask for template match (3 channel)
    
    // eroded mask is used to ensure that psiHatQ is not overlapping with target
    cv::erode(maskMat, erodedMask, cv::Mat(), cv::Point(-1, -1), RADIUS);
    
    cv::Mat drawMat;
    
    
    // main loop
    const size_t area = maskMat.total();
    
    while (cv::countNonZero(maskMat) != area)   // end when target is filled
    {
        // set priority matrix to -.1, lower than 0 so that border area is never selected
        priorityMat.setTo(-0.1f);
        
        // get the contours of mask
        getContours((maskMat == 0), contours, hierarchy);
        
        if (DEBUG) {
            drawMat = colorMat.clone();
        }
        
        // compute the priority for all contour points
        computePriority(contours, grayMat, confidenceMat, priorityMat);
        
        // get the patch with the greatest priority
        cv::minMaxLoc(priorityMat, NULL, NULL, NULL, &psiHatP);
        psiHatPColor = getPatch(colorMat, psiHatP);
        psiHatPConfidence = getPatch(confidenceMat, psiHatP);
        
        cv::Mat confInv = (psiHatPConfidence != 0.0f);
        confInv.convertTo(confInv, CV_16UC1);
        confInv /= 255.0f;
        // get the patch in source with least distance to psiHatPColor wrt source of psiHatP
        cv::Mat mergeArrays[3] = {confInv, confInv, confInv};
        cv::merge(mergeArrays, 3, templateMask);
        result = computeSSD(psiHatPColor, colorMat, templateMask);
        
        // set all target regions to 1.1, which is over the maximum value possilbe
        // from SSD
        result.setTo(1.1f, erodedMask == 0);
        // get minimum point of SSD between psiHatPColor and colorMat
        cv::minMaxLoc(result, NULL, NULL, &psiHatQ);
        
        assert(psiHatQ != psiHatP);
        
        if (DEBUG) {
            cv::rectangle(drawMat, psiHatP - cv::Point(RADIUS, RADIUS), psiHatP + cv::Point(RADIUS+1, RADIUS+1), cv::Scalar(255, 0, 0));
            cv::rectangle(drawMat, psiHatQ - cv::Point(RADIUS, RADIUS), psiHatQ + cv::Point(RADIUS+1, RADIUS+1), cv::Scalar(0, 0, 255));
            showMat("red - psiHatQ", drawMat);
        }
        // updates
        // copy from psiHatQ to psiHatP for each colorspace
        transferPatch(psiHatQ, psiHatP, grayMat, (maskMat == 0));
        transferPatch(psiHatQ, psiHatP, colorMat, (maskMat == 0));
        
        // fill in confidenceMat with confidences C(pixel) = C(psiHatP)
        confidence = computeConfidence(psiHatPConfidence);
        assert(0 <= confidence && confidence <= 1.0f);
        // update confidence
        psiHatPConfidence.setTo(confidence, (psiHatPConfidence == 0.0f));
        // update maskMat
        maskMat = (confidenceMat != 0.0f);
    }
    
    showMat("final result", colorMat, 0);
    return 0;
}

/*
//Image Edit
int main(){
    string path="/Users/estelle/Project/3DFaceReconstruction/final_gradient_test_result.png";
    string out_path="/Users/estelle/Project/3DFaceReconstruction/mask.png";
    
    Mat image = imread(path,CV_16UC1);
    Mat out_image = image.clone();
    int rows = out_image.rows;
    int cols = out_image.cols;

    for (int i=0;i<rows;i++){
        int startPoint=-1;//depth info
        int endPoint=-1;
        int startcols=-1;//position
        int endcols=-1;
        for (int j=0;j<cols;j++){
            unsigned short int start = (unsigned short int) image.at<ushort>(i,j);
            if(start != 0){
                startPoint = start;
                startcols = j+5;
                for (int x=cols-1;x>=j;x--){
                    unsigned short int end = (unsigned short int) image.at<ushort>(i,x);
                    if (end != 0){
                        endPoint = end;
                        endcols = x;
                        break;
                    }
                }
                break;
            }
        }

        if(startcols!=-1 && endcols!=-1){

            for (int m=0;m<=startcols;m++){
                double temp=0.0;
                out_image.at<ushort>(i,m) = temp;
            }
            for (int n=endcols;n<cols;n++){
                double black=0.0;
                out_image.at<ushort>(i,n) = black;
            }
            for (int p = endcols; p>startcols;p--){
                double white=65535.0;
                out_image.at<ushort>(i,p) = white;
            }
        }
    }
//    for (int i=0;i<rows;i++){
//        for (int j=0;j<cols;j++){
//            unsigned short int current = (unsigned short int) out_image.at<ushort>(i,j);
//            out_image.at<ushort>(i,j) = (unsigned short int)65535-current;
//        }
//    }
    
    namedWindow("display window",WINDOW_AUTOSIZE);
    imshow("display window", out_image);
    imwrite(out_path, out_image);
    //waitKey();
    return 0;
}
*/
