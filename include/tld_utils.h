#include <opencv2/opencv.hpp>
#pragma once

using namespace cv;
using namespace std;

//#define __DEBUG__
#ifdef __DEBUG__
#define DEBUG(format,...) printf("File: "__FILE__", Line: %05d: "format"/n", __LINE__, ##__VA_ARGS__)
#else
#define DEBUG(format,...)
#endif

void drawBox(cv::Mat& image, CvRect box, cv::Scalar color = cvScalarAll(255), int thick=1); 

void drawPoints(cv::Mat& image, std::vector<cv::Point2f> points,cv::Scalar color=cv::Scalar::all(255));

cv::Mat createMask(const cv::Mat& image, CvRect box);

float median(std::vector<float> v);

std::vector<int> index_shuffle(int begin,int end);
void myResize(const unsigned char *dataSrc, unsigned char *dataDst, int src_width, int src_height, int width, int height);
//double myTemplateMatch(const Mat * pTemplate, const Mat * src, int w, int h);
float myTemplateMatch(const Mat *pTemplate,const Mat *src,int method ); /*method : 1 CV_TM_CCORR_NORMED  2 CV_TM_CCOEFF_NORMED*/


