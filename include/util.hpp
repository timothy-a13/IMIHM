#ifndef __UTIL_H__
#define __UTIL_H__

#include <filesystem>
#include <opencv2/opencv.hpp>

cv::Mat applyTransformation(const cv::Mat &image, const cv::Mat &tfmMatrix);
cv::Mat applyTransformation(const cv::Mat &image, const cv::Mat &tfmMatrix, const cv::Size &imgSize);
void calculateOverlap(cv:: Mat &overlap, int &ovWidth, double &ovRate, 
                      const cv::Mat &image, const cv::Size &size);
void calculateOvWidthAndOvRate(cv:: Mat &overlap, int &ovWidth, double &ovRate);
void clearFolder(const std::string& folderPath);

#endif   // __UTIL_H__