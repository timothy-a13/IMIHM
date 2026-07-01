#include "hm.hpp"

// 計算直方圖
void calculateHistogram(std::array<double, 256> &hist, const cv::Mat &image, int channel) {
    int overlap_pixels = 0; // 計算重疊區的pixel數
    for (int i = 0; i < 256; i++) {
        hist[i] = 0;
    }

    for (int i = 0; i < image.rows; i++) {
        for (int j = 0; j < image.cols; j++) {
            hist[(int)image.at<cv::Vec3b>(i, j)[channel]]++;
            overlap_pixels++;
        }
    }
}

// 計算直方圖，只有overlap_region的pixel為白的才記
void calculateHistogram(std::array<double, 256> &hist, const cv::Mat &image,
                        const cv::Mat &overlap, int channel) {
    int overlap_pixels = 0; // 計算重疊區的pixel數
    for (int i = 0; i < 256; i++) {
        hist[i] = 0;
    }

    for (int i = 0; i < image.rows; i++) {
        for (int j = 0; j < image.cols; j++) {
            int x = (int)overlap.at<uchar>(i, j);
                
            if (x == 255) {
                //std::cout << "(" << i << ", " << j << "): " << x << std::endl;
                hist[(int)image.at<cv::Vec3b>(i, j)[channel]]++;
                overlap_pixels++;
            }
        }
    }
}

// 計算累積直方圖
void histogramToCDF(std::array<double, 256> &CDF, const std::array<double, 256> &histogram) {
    for (int i = 1; i < 256; i++) {
        CDF[i] = 0.f;
    }
    CDF[0] = histogram[0];
    for (int i = 1; i < 256; i++) {
        CDF[i] += histogram[i] + CDF[i - 1];
    }
}

// 計算映射的函式，較低處無對應值對應值，可能須修改(當reference的亮度相對低時)
void calculateMapFunction(std::array<int, 256> &mappingResult, 
                          const std::array<double, 256> &srcCDF,
                          const std::array<double, 256> &tgtCDF,
                          const std::array<double, 256> &histogram) {
    
    char flag = '0';                  // 只要記錄第一個，所以需要一個開關
    int temp_i = -100, temp_j = -100; // 紀錄第一個正常映射及正常被映射的值

    for (int i = 0; i < 256; i++) {
        for (int j = 0; j < 256; j++) {
            if (srcCDF[j] > tgtCDF[i]) {
                if (flag == '0' && (j - 1) >= 0) {
                    flag = '1'; // 開關打開!
                    temp_i = i;
                    temp_j = j - 1;
                }

                mappingResult[i] = (int)cv::saturate_cast<uchar>(j - 1);
                break;
            }
            mappingResult[i] = mappingResult[i - 1]; // 當無值可映射時取前一個映射值
        }
    }
    // cout<<temp_i<<' '<<temp_j<<endl;

    // 當無值可映射時取第一個正常映射的值
    for (int i = temp_i; i >= 0; i--) {
        mappingResult[i] = temp_j;
    }

    double sum1 = 0, sum2 = 0;
    // 求M[0]-0的質心
    for (int i = 0; i <= mappingResult[0]; i++) {
        sum1 += histogram[i];
        sum2 += histogram[i] * i;
    }
    mappingResult[0] = sum2 / sum1;

    sum1 = 0, sum2 = 0;
    // 求M[255]-255的質心
    for (int i = 255; i >= mappingResult[255]; i--) {
        sum1 += histogram[i];
        sum2 += histogram[i] * i;
    }
    mappingResult[255] = sum2 / sum1;
}

// 將圖片依映射函數更新顏色
void correctColorWithMap(cv::Mat &tgtImg, std::array<int, 256> &colorMap, int channel) {
    for (int i = 0; i < tgtImg.rows; i++) {
        for (int j = 0; j < tgtImg.cols; j++) {
            if (tgtImg.at<cv::Vec3b>(i, j)[0] == 0 &&
                tgtImg.at<cv::Vec3b>(i, j)[1] == 0 &&
                tgtImg.at<cv::Vec3b>(i, j)[2] == 0) continue;
            tgtImg.at<cv::Vec3b>(i, j)[channel] = colorMap[tgtImg.at<cv::Vec3b>(i, j)[channel]];
        }
    }
}

void hmColorCorrection(const cv::Mat &srcAlignedImg, cv::Mat &tgtImage, 
                        const cv::Mat &overlapImg) {
    
    cv::Mat srcAlignedImgYUV, tgtImgYUV, orgTgtImgYUV;
    cvtColor(srcAlignedImg, srcAlignedImgYUV, cv::COLOR_BGR2YCrCb); // BGR轉YUV
    cvtColor(tgtImage, tgtImgYUV, cv::COLOR_BGR2YCrCb);
    orgTgtImgYUV = tgtImgYUV.clone();

    for (int i = 0; i < 3; i++) {
        std::array<double, 256> srcHistogram;
        std::array<double, 256> tgtHistogram;

        std::array<double, 256> srcCDF;
        std::array<double, 256> tgtCDF;

        std::array<int, 256> colorMap;

        calculateHistogram(srcHistogram, srcAlignedImgYUV, overlapImg, i);
        calculateHistogram(tgtHistogram, tgtImgYUV, overlapImg, i);   //

        histogramToCDF(srcCDF, srcHistogram);
        histogramToCDF(tgtCDF, tgtHistogram);

        calculateMapFunction(colorMap, srcCDF, tgtCDF, srcHistogram);
        correctColorWithMap(tgtImgYUV, colorMap, i);
    }

    cv::Mat finalResult = tgtImgYUV.clone();
    cvtColor(tgtImgYUV, finalResult, cv::COLOR_YCrCb2BGR);
    cv::addWeighted(finalResult, 0.94, tgtImage.clone(), 0.06, 0, tgtImage);
}

void hmColorCorrection(const cv::Mat &srcAlignedImg, cv::Mat &tgtImage) {
    
    cv::Mat srcAlignedImgYUV, tgtImgYUV;
    cvtColor(srcAlignedImg, srcAlignedImgYUV, cv::COLOR_BGR2YCrCb); // BGR轉YUV
    cvtColor(tgtImage, tgtImgYUV, cv::COLOR_BGR2YCrCb);

    for (int i = 0; i < 3; i++) {
        std::array<double, 256> srcHistogram;
        std::array<double, 256> tgtHistogram;

        std::array<double, 256> srcCDF;
        std::array<double, 256> tgtCDF;

        std::array<int, 256> colorMap;

        calculateHistogram(srcHistogram, srcAlignedImgYUV, i);
        calculateHistogram(tgtHistogram, tgtImgYUV, i);   //

        histogramToCDF(srcCDF, srcHistogram);
        histogramToCDF(tgtCDF, tgtHistogram);

        calculateMapFunction(colorMap, srcCDF, tgtCDF, srcHistogram);
        correctColorWithMap(tgtImgYUV, colorMap, i);
    }

    cvtColor(tgtImgYUV, tgtImage, cv::COLOR_YCrCb2BGR); // YUV轉BGR
}
