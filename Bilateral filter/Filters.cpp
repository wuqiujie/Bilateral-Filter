//
//  Filters.cpp
//  Bilateral filter
//
//  Created by 吴秋洁 on 2019/6/16.
//  Copyright © 2019年 吴秋洁. All rights reserved.
//

#include "Filters.hpp"
using namespace std;
using namespace cv;



double SpaceFactor(int x1, int y1, int x2, int y2, double sigmaD) {
    double absX = pow(abs(x1 - x2), 2);
    double absY = pow(abs(y1 - y2), 2);
    
    return exp(-(absX + absY) / (2 * pow(sigmaD, 2)));
}

double ColorFactor(int x, int y, double sigmaR) {
    double distance = abs(x - y) / sigmaR;
    return exp(-0.5 * pow(distance, 2));
}

Mat BilateralFilter(Mat inputImg, int filterSize, double sigmaD, double sigmaR) {
    int len; //must be odd number
    Mat gray; // must be 1-channel image
    Mat LabImage; // if channels == 3
    
    if (filterSize % 2 != 1 || filterSize <= 0) {
        cerr << "Filter Size must be a positive odd number!" << endl;
        return inputImg;
    }
    len = filterSize / 2;
    
    if (inputImg.channels() >= 3) {
        cvtColor(inputImg, LabImage, COLOR_BGR2Lab);
        gray = Mat::zeros(LabImage.size(), CV_8UC1);
        for (int i = 0; i < LabImage.rows; i++) {
            for (int j = 0; j < LabImage.cols; j++) {
                gray.ptr<uchar>(i)[j] = LabImage.ptr<uchar>(i, j)[0];
            }
        }
    }
    else if(inputImg.channels() == 1){
        inputImg.copyTo(gray);
    }
    else {
        cerr << "the count of input image's channel can not be 2!" << endl;
        return inputImg;
    }
    
    Mat resultGrayImg = Mat::zeros(gray.size(), CV_8UC1);
    for (int i = 0; i < gray.rows; i++) {
        for (int j = 0; j < gray.cols; j++) {
            double k = 0;
            double f = 0;
            for (int r = i - len; r <= i + len; r++) {
                for (int c = j - len; c <= j + len; c++) {
                    if (r < 0 || c < 0 || r >= gray.rows || c >= gray.cols)
                        continue;
                    f = f + gray.ptr<uchar>(r)[c] * SpaceFactor(i, j, r, c, sigmaD) * ColorFactor(gray.ptr<uchar>(i)[j], gray.ptr<uchar>(r)[c], sigmaD);
                    k += SpaceFactor(i, j, r, c, sigmaD) * ColorFactor(gray.ptr<uchar>(i)[j], gray.ptr<uchar>(r)[c], sigmaD);
                }
            }
            int value = f / k;
            if (value < 0) value = 0;
            else if (value > 255) value = 255;
            
            resultGrayImg.ptr<uchar>(i)[j] = (uchar)value;
        }
    }
    
    Mat resultImg;
    if (inputImg.channels() >= 3) {
        for (int i = 0; i < LabImage.rows; i++) {
            for (int j = 0; j < LabImage.cols; j++) {
                LabImage.ptr<uchar>(i, j)[0] = resultGrayImg.ptr<uchar>(i)[j];
            }
        }
        cvtColor(LabImage, resultImg, COLOR_Lab2BGR);
    }
    else {
        resultGrayImg.copyTo(resultImg);
    }
    
    return resultImg;
}
