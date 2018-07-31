//
//  Reference_line.hpp
//  openCVTest
//
//  Created by Estelle on 2018/7/6.
//  Copyright © 2018 Estelle. All rights reserved.
//

#ifndef Reference_line_hpp
#define Reference_line_hpp

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
#include "Image_filter.hpp"

using namespace std;

int reference_line(int x, int y, int z, Mat model);

#endif /* Reference_line_hpp */
