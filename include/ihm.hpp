#ifndef __IHM_H__
#define __IHM_H__

#include <opencv2/opencv.hpp>

void colorCorrectIHM(const cv::Mat &srcAlignedImg, const cv::Mat &tgtAlignedImg,
                     cv::Mat &srcImage, cv::Mat &tgtImage, const cv::Mat &tgtTfm);

#endif   // __IHM_H__