#include "loader.hpp"

#include <fstream>

int loadImage(const std::string &filename, cv::Mat &outImage, double sizeFactor) {
    cv::Mat img = cv::imread(filename, cv::IMREAD_COLOR);
    if (img.empty()) return -1;
    cv::resize(
        img, outImage, 
        cv::Size(img.cols * sizeFactor, img.rows * sizeFactor),
        0, 0, cv::INTER_AREA
    );

    return 0;
}

cv::Mat loadTfmMatrix(const std::string &filename) {
    std::ifstream file(filename);
    cv::Mat tranformationMatrix(3, 3, CV_64F);
    if (file.is_open()) {
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                file >> tranformationMatrix.at<double>(i, j);
        file.close();
    } else {
        throw std::runtime_error("Unable to open file " + filename);
    }
    return tranformationMatrix;
}
