#include "ihm.hpp"
#include "hm.hpp"
#include "util.hpp"
#include "global_folder_paths.hpp"

#include <iostream>
#include <opencv2/opencv.hpp>

void colorCorrectIHM(const cv::Mat &srcAlignedImg, const cv::Mat &tgtAlignedImg,
                     cv::Mat &srcImage, cv::Mat &tgtImage, const cv::Mat &tgtTfm) {
    // Initial the first map
    int overlapWidth;
    double overlapRate;
    cv::Mat overlapImg;
    calculateOverlap(overlapImg, overlapWidth, overlapRate, tgtAlignedImg, srcImage.size());
    //std::cout << (int)overlapImg.at<bool>(300, 300) << " " << overlapWidth << " " << overlapRate << std::endl;
    cv::imwrite(tmpFolder + "overlap1.png", overlapImg);

    cv::Mat invTgtTfm;
    cv::invert(tgtTfm, invTgtTfm);
    overlapImg = applyTransformation(overlapImg, invTgtTfm);
    cv::imwrite(tmpFolder + "overlap2.png", overlapImg);

    cv::imwrite(tmpFolder + "aligned_output4.png", applyTransformation(srcImage, invTgtTfm));

    calculateOvWidthAndOvRate(overlapImg, overlapWidth, overlapRate);
    double windowsOvRate = 0.8;
    int biggestOvRateInd, I = 9,
        M = tgtImage.cols / ((1 - windowsOvRate) * (I - 1) + 1),
        deltaX = M * (1 - windowsOvRate);
    
    double biggestRate = 0.;
    std::cout << "deltaX: " << deltaX << " M: " << M << std::endl;
    for (int i = 0; i < I; i++) {
        cv::Rect subArea = cv::Rect(i * deltaX, 0, MIN(M, tgtImage.cols - i * deltaX), tgtImage.rows);
        //std::cout << "Width " << subArea.width << std::endl;
        cv::Mat subOvImg = overlapImg(subArea);
        int subOvWidth; double subOvRate;
        calculateOvWidthAndOvRate(subOvImg, subOvWidth, subOvRate);
        if (subOvRate > biggestRate) biggestOvRateInd = i;
        if (i * deltaX + M > tgtImage.cols) break;
    }

    std::cout << "Biggest Index: " << biggestOvRateInd << std::endl;

    std::cout << "M: " << M << " I: " << I << " dX: " << deltaX << std::endl;

    cv::Mat newTgtImg = tgtImage.clone();

    cv::Rect subArea = cv::Rect(biggestOvRateInd * deltaX, 0, 
                                MIN(M, tgtImage.cols - biggestOvRateInd * deltaX), tgtImage.rows);
    cv::Mat initSliced = tgtImage(subArea).clone();
    hmColorCorrection(applyTransformation(srcImage, invTgtTfm)(subArea), initSliced, overlapImg(subArea));

    initSliced.copyTo(newTgtImg(subArea));

    cv::Mat shiftMat, slicedOverlap;
    cv::Rect overlapRagn;

    shiftMat = (cv::Mat_<double>(2, 3) << 1, 0, deltaX, 0, 1, 0);
    slicedOverlap = cv::Mat::zeros(tgtImage.rows, M, CV_8UC1);
    overlapRagn = cv::Rect(deltaX, 0, M * windowsOvRate, tgtImage.rows);
    slicedOverlap(overlapRagn).setTo(cv::Scalar(255));
    cv::imwrite(tmpFolder + "slicedOverlap1.png", slicedOverlap);
    for (int i = biggestOvRateInd - 1; i >= 0; i--) {
        cv::Rect subArea = cv::Rect(i * deltaX, 0, 
                                    MIN(M, i * deltaX + M), tgtImage.rows);
        cv::Rect preSubArea = cv::Rect((i + 1) * deltaX, 0, M, tgtImage.rows);
        cv::Mat sliced = tgtImage(subArea).clone(), preSliced = newTgtImg(preSubArea).clone();
        cv::imwrite(tmpFolder + "subarea" + std::to_string(i + 1) + ".png", tgtImage(preSubArea).clone());
        cv::warpAffine(preSliced, preSliced, shiftMat, preSliced.size());
        hmColorCorrection(preSliced, sliced, slicedOverlap);

        sliced.copyTo(newTgtImg(subArea));
        cv::imwrite(tmpFolder + "sliced" + std::to_string(i + 1) + ".png", sliced);
        //cv::imwrite("sliced.png", sliced);
    }

    shiftMat = (cv::Mat_<double>(2, 3) << 1, 0, -deltaX, 0, 1, 0);
    slicedOverlap = cv::Mat::zeros(tgtImage.rows, M, CV_8UC1);
    slicedOverlap(cv::Rect(0, 0, M * windowsOvRate, tgtImage.rows)).setTo(cv::Scalar(255));
    cv::imwrite(tmpFolder + "slicedOverlap2.png", slicedOverlap);
    for (int i = biggestOvRateInd + 1; i < I; i++) {
        cv::Rect subArea = cv::Rect(i * deltaX, 0, MIN(M, tgtImage.cols - i * deltaX), tgtImage.rows);
        cv::Rect preSubArea = cv::Rect((i - 1) * deltaX, 0, M, tgtImage.rows);
        cv::Mat sliced = tgtImage(subArea).clone(), preSliced = tgtImage(preSubArea).clone();
        cv::imwrite(tmpFolder + "subarea" + std::to_string(i - 1) + ".png", preSliced);
        cv::warpAffine(preSliced, preSliced, shiftMat, preSliced.size());
        hmColorCorrection(preSliced, sliced, slicedOverlap);

        sliced.copyTo(newTgtImg(subArea));
        cv::imwrite(tmpFolder + "sliced.png", sliced);
    }

    tgtImage = newTgtImg;
}
