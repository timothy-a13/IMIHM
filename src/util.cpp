#include "util.hpp"

cv::Mat applyTransformation(const cv::Mat &image, const cv::Mat &tfmMatrix) {
    cv::Mat transformedImage;
    cv::warpPerspective(image, transformedImage, tfmMatrix, image.size());
    return transformedImage;
}

cv::Mat applyTransformation(const cv::Mat &image, const cv::Mat &tfmMatrix, const cv::Size &imgSize) {
    cv::Mat transformedImage;
    cv::warpPerspective(image, transformedImage, tfmMatrix, imgSize);
    return transformedImage;
}

void calculateOverlap(cv:: Mat &overlap, int &ovWidth, double &ovRate, 
                      const cv::Mat &image, const cv::Size &size) {
    overlap = cv::Mat(image.size(), CV_8UC1, cv::Scalar(0));
    int ovNum = 0, left = overlap.cols, right = 0;
    for (int i = 0; i < image.rows; i++)
        for (int j = 0; j < image.cols; j++)
            if (image.at<cv::Vec3b>(i, j)[0] + image.at<cv::Vec3b>(i, j)[1] +
                image.at<cv::Vec3b>(i, j)[2] != 0) {
                    overlap.at<uchar>(i, j) = 255;
                    ovNum++;
                    if (i < left) left = i;
                    else if (i > right) right = i;
                }
                
    ovWidth = right - left;
    ovRate = (double) ovNum / (image.rows * image.cols);
    return ;
}

void calculateOvWidthAndOvRate(cv:: Mat &overlap, int &ovWidth, double &ovRate) {
    int ovNum = 0, left = overlap.cols, right = 0;
    for (int i = 0; i < overlap.rows; i++)
        for (int j = 0; j < overlap.cols; j++)
            if (((int)overlap.at<uchar>(i, j)) == 255) {
                ovNum++;
                if (j < left) left = j;
                else if (j > right) right = j;
            }

    ovWidth = right - left;
    ovRate = (double) ovNum / (overlap.rows * overlap.cols);
    return ;
}


void clearFolder(const std::string& folderPath) {
    namespace fs = std::filesystem;
    if (fs::exists(folderPath) && fs::is_directory(folderPath))
        for (const auto& entry : fs::directory_iterator(folderPath))
            fs::remove_all(entry.path());
}
