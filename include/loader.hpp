#ifndef __LOADER_H__
#define __LOADER_H__

#include <string>
#include <opencv2/opencv.hpp>

int loadImage(const std::string &filename, cv::Mat &outImage, double sizeFactor = 1.);
cv::Mat loadTfmMatrix(const std::string &filename);

#endif   // __LOADER_H__