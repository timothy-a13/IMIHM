#ifndef __HM_H__
#define __HM_H__

#include <array>
#include <cmath>
#include <opencv2/opencv.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/imgproc/imgproc.hpp>

void calculateHistogram(std::array<double, 256> &hist, const cv::Mat &image, int channel);
void calculateHistogram(std::array<double, 256> &hist, const cv::Mat &image,
                        const cv::Mat &overlap, int channel);

void histogramToCDF(std::array<double, 256> &CDF, const std::array<double, 256> &histogram);

void calculateMapFunction(std::array<int, 256> &mappingResult, 
                          const std::array<double, 256> &srcCDF,
                          const std::array<double, 256> &tgtCDF,
                          const std::array<double, 256> &histogram);

void correctColorWithMap(cv::Mat &tgtImg, std::array<int, 256> &colorMap, int channel);

void hmColorCorrection(const cv::Mat &srcAlignedImg, cv::Mat &tgtImage);
void hmColorCorrection(const cv::Mat &srcAlignedImg, cv::Mat &tgtImage, 
                        const cv::Mat &overlapImg);

#endif   // __HM_H__