 //
//  Filters.hpp
//  Bilateral filter
//
//  Created by 吴秋洁 on 2019/6/16.
//  Copyright © 2019年 吴秋洁. All rights reserved.
//

#ifndef Filters_hpp
#define Filters_hpp

#include <stdio.h>

#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/core.hpp"


#include <iostream>
#include <cmath>
using namespace std;
using namespace cv;


//Bilateral Filtering
//sigmaD == sigmaSpace, sigmaR == sigmaColor
Mat BilateralFilter(Mat inputImg, int filterSize, double sigmaD, double sigmaR);


#endif // ! FILTERS_H
