//
//  BilateralFilter.hpp
//  Bilateral filter
//
//  Created by 吴秋洁 on 2019/6/16.
//  Copyright © 2019年 吴秋洁. All rights reserved.
//

#ifndef BilateralFilter_hpp
#define BilateralFilter_hpp

#include <stdio.h>



#include "cv.h"
#include "cxcore.h"

class BilateralFilter{
public:
    IplImage * image;
    IplImage * rimage ;
    
    double kernelRadius;
    double ** kernelD;
    double *gaussSimilarity;
    
    BilateralFilter(IplImage *image,double sigmaD, double sigmaR);
    IplImage * runFilter();
    void apply(int i,int j);
    bool isInsideBoundaries(int m,int n);
    double similarity(int p,int s);
    double gauss(double sigma,int x, int y);
    double BilateralFilter::getSpatialWeight(int m, int n,int i,int j);
};
#endif /* BilateralFilter_hpp */
