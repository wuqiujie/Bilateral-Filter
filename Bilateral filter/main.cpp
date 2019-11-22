//  Bilateral filter
//
//  Created by 吳秋潔 on 2019/6/10.
//  Copyright © 2019年 吳秋潔. All rights reserved.
//

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <cstdio>



using namespace std;
using namespace cv;

/* 空間權重 */
double **get_space_Array( int size, int channels, double s)

{
    int i, j;
    double **spaceArray = new double*[size+1];
    for (i = 0; i < size+1; i++) {
        spaceArray[i] = new double[size+1];
    }
   
    int center_i, center_j;
    center_i = center_j = size / 2;
    spaceArray[size][0] = 0.0f;
    
   
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            spaceArray[i][j] =exp(-(1.0f)* (((i - center_i)*(i - center_i) + (j - center_j)*(j - center_j)) /(2.0f*s*s)));
            spaceArray[size][0] += spaceArray[i][j];
        }
    }
    return spaceArray;
}

/* 光度色彩差異權重 */
double *get_color_Array(int size, int channels, double c)
{
    
    int n;
    double *colorArray = new double[256 * channels + 2];
    colorArray[256 * channels+1 ] = 0.0f;
    
    for (n = 0; n < 256 * channels + 2; n++) {
        colorArray[n] = exp((-1.0f*(n*n)) / (2.0f*c*c));
        colorArray[256 * channels +1] += colorArray[n];
    }
    return colorArray;
}


void doBialteral(Mat *src, int N, double *colorArray, double **spaceArray)
{
    int size = 2 * N + 1;
    Mat temp = (*src).clone();
    
    
    for (int i = 0; i < (*src).rows; i++) {
        for (int j = 0; j < (*src).cols; j++) {
          
            if (i > (size / 2) - 1 && j > (size / 2) - 1 &&
                i < (*src).rows - (size / 2) && j < (*src).cols - (size / 2)) {
                
            
                double sum[3] = { 0.0,0.0,0.0 };
                int x, y, values;
                double space_color_sum = 0.0f;
                
                

                for (int q = 0; q < size; q++) {
                    for (int p = 0; p < size; p++) {
                        x = i - q + (size / 2);
                        y = j - p + (size / 2);
                        
                    values = abs((*src).at<Vec3b>(i, j)[0] + (*src).at<Vec3b>(i, j)[1] + (*src).at<Vec3b>(i, j)[2]
                            - (*src).at<Vec3b>(x, y)[0] - (*src).at<Vec3b>(x, y)[1] - (*src).at<Vec3b>(x, y)[2]);
                        
                        space_color_sum += (colorArray[values] * spaceArray[q][p]);
                    }
                }
                
            
                for (int q = 0; q < size; q++) {
                    for (int p = 0; p < size; p++) {
                        x = i - q + (size / 2);
                        y = j - p + (size / 2);
                        values = abs((*src).at<Vec3b>(i, j)[0] + (*src).at<Vec3b>(i, j)[1] + (*src).at<Vec3b>(i, j)[2]
                                     - (*src).at<Vec3b>(x, y)[0] - (*src).at<Vec3b>(x, y)[1] - (*src).at<Vec3b>(x, y)[2]);
                        
                        
                        for (int c = 0; c < 3; c++) {
                            sum[c] += ((*src).at<Vec3b>(x, y)[c]* colorArray[values]* spaceArray[q][p])/ space_color_sum;
                        }
                        
                        
                    }
                }
                for (int c = 0; c < 3; c++) {
                    temp.at<Vec3b>(i, j)[c] = sum[c];
                }
            }
        }
    }
    
    (*src) = temp.clone();
    
    return ;
}

/* 雙邊濾波 */
void myBialteralFilter(Mat *src, Mat *dst, int N, double c, double s)
{
    
    *dst = (*src).clone();
    int size = 2 * N + 1;
    double *colorArray = NULL;
    double **spaceArray = NULL;
    
  
    int channels = (*dst).channels();
    colorArray = get_color_Array(size, channels, c);
    spaceArray = get_space_Array(size, channels, s);
    
    
    doBialteral(dst, N, colorArray, spaceArray);
    
    
    return;
}





int main(){
   // Mat src = imread("/Users/wuqiujie/Desktop/old.JPG");
    Mat src = imread("/Users/wuqiujie/Desktop/beauty.JPG");
    
   
   
    Mat dst1;
    Mat dst2;
   
    /*
     Mat d1;
     Mat d2;
     Mat d3;
     Mat d4;
    
     Mat d5;
     Mat d6;
     Mat d7;
     Mat d8;
    
     Mat d9;
     Mat d10;
     Mat d11;
     Mat d12;
    */
  
   
    //bilateralFilter(src, dst1, 5 ,100 ,100);
    myBialteralFilter(&src, &dst2, 5, 50, 50);
    
    
  /*
    myBialteralFilter(&src, &d1, 1, 10, 300);
    myBialteralFilter(&src, &d2, 1, 30, 300);
    myBialteralFilter(&src, &d3, 1, 100, 300);
    myBialteralFilter(&src, &d4, 1, 300, 300);
    
    myBialteralFilter(&src, &d5, 3, 10, 300);
    myBialteralFilter(&src, &d6, 3, 30, 300);
    myBialteralFilter(&src, &d7, 3, 100, 300);
    myBialteralFilter(&src, &d8, 3, 300, 300);
    
    myBialteralFilter(&src, &d9, 10, 10, 300);
    myBialteralFilter(&src, &d10, 10, 30, 300);
    myBialteralFilter(&src, &d11, 10, 100, 300);
    myBialteralFilter(&src, &d12, 10, 300, 300);
 */
    
 
    imshow("OriginlPicture", src);
//    imshow("OpenCVBialteralFilter", dst1);
    imshow("myBialteralFilter", dst2);
  
    
    /*
    imshow("myBialteralFilter1", d1);
     imshow("myBialteralFilter2", d2);
     imshow("myBialteralFilter3", d3);
     imshow("myBialteralFilter4", d4);
    
     imshow("myBialteralFilter5", d5);
     imshow("myBialteralFilter6", d6);
     imshow("myBialteralFilter7", d7);
     imshow("myBialteralFilter8", d8);
    
     imshow("myBialteralFilter9", d9);
     imshow("myBialteralFilter10", d10);
     imshow("myBialteralFilter11", d11);
     imshow("myBialteralFilter12", d12);
  */
    waitKey(0);
    
    return 0;
}

