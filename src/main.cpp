#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>
#include <algorithm>
#include <opencv2/opencv.hpp>

#include "loader.hpp"
#include "hm.hpp"
#include "util.hpp"
#include "ihm.hpp"
#include "global_folder_paths.hpp"



int main(int argc, char** argv) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <Image Folder> <Tfm Matrix Folder> <Number of Image>" << std::endl;
        return -1;
    }

    

    int numOfImage = atoi(argv[3]);
    std::vector<std::string> imagePaths, tfmPaths;

    namespace fs = std::filesystem;

    for (const auto& entry : fs::directory_iterator(argv[1])) {
        imagePaths.push_back(entry.path().string());
        std::cout << entry.path().string() << std::endl;
    }

    for (const auto& entry : fs::directory_iterator(argv[2])) {
        tfmPaths.push_back(entry.path().string());
        std::cout << entry.path().string() << std::endl;
    }

    if (imagePaths.size() != numOfImage || tfmPaths.size() != numOfImage + 2) {
        std::cerr << "Argument Error" << std::endl;
        return -1;
    }

    std::sort(imagePaths.begin(), imagePaths.end());
    std::sort(tfmPaths.begin(), tfmPaths.end());

    clearFolder(tmpFolder);
    clearFolder(orgIhmFolder);
    clearFolder(warpedFolder);

    cv::Mat shiftMat = loadTfmMatrix(tfmPaths[tfmPaths.size() - 1]);
    std::cout << shiftMat << std::endl;
    
    cv::Size warpedImgSize;
    std::ifstream shapeFile(tfmPaths[tfmPaths.size() - 2]);
    shapeFile >> warpedImgSize.width >> warpedImgSize.height;



    for (int i = 1; i < numOfImage; i++) {
        // 讀取圖片
        cv::Mat srcImage, tgtImage;
        double sizeFactor = 1 / 4.;

        bool imgLoadFail = false;
        if (i == 1) imgLoadFail |= (loadImage(imagePaths[i - 1], srcImage, sizeFactor) == -1);
        else imgLoadFail |= (
            loadImage(orgIhmFolder + std::to_string(i-1).insert(0, 1 - (bool)((i-1) / 10), '0') + ".JPG", srcImage, 1.) == -1
        );
        imgLoadFail |= (loadImage(imagePaths[i], tgtImage, sizeFactor) == -1);

        if (imgLoadFail) {
            std::cerr << "Error loading images" << std::endl;
            return -1;
        }
        std::cout << "Image Size: (" << srcImage.size << ")" << std::endl;

        // 讀取旋轉矩陣
        cv::Mat srcTfmMat = loadTfmMatrix(tfmPaths[i - 1]);
        cv::Mat tgtTfmMat = loadTfmMatrix(tfmPaths[i]);
        std::cout << tgtTfmMat << std::endl;

        // 將旋轉矩陣應用到圖片
        cv::Mat srcAlignedImg = srcImage.clone(); //applyTransformation(srcImage, srcTfmMat);
        cv::Mat invSrcTfm;
        cv::invert(srcTfmMat, invSrcTfm, cv::DECOMP_LU);
        cv::Mat tgtAlignedTfmMat = invSrcTfm * tgtTfmMat;
        // Test if identity matrix
        std::cout << "invTfmMat * srcTfm: " << invSrcTfm * srcTfmMat << std::endl;
        cv::Mat tgtAlignedImg = applyTransformation(tgtImage, tgtAlignedTfmMat);

        colorCorrectIHM(srcAlignedImg, tgtAlignedImg, srcImage, tgtImage, tgtAlignedTfmMat);

        cv::Mat finalImage;
        cv::addWeighted(srcAlignedImg, 0.5, tgtAlignedImg, 0.5, 0.0, finalImage);

        cv::imwrite(tmpFolder + "aligned_output.png", finalImage);
        cv::imwrite(tmpFolder + "aligned_output1.png", srcAlignedImg);
        cv::imwrite(tmpFolder + "aligned_output2.png", tgtAlignedImg);
        //cv::resize(tgtImage, tgtImage, cv::Size(tgtImage.cols / sizeFactor, tgtImage.rows / sizeFactor), 0, 0, cv::INTER_CUBIC);
        //cv::imwrite(tmpFolder + "aligned_output3.JPG", tgtImage);

        cv::imwrite(
            orgIhmFolder +
            std::to_string(i).insert(0, 1 - (bool)(i / 10), '0') + ".JPG",
            tgtImage
        );

        cv::imwrite(
            warpedFolder +
            std::to_string(i).insert(0, 1 - (bool)(i / 10), '0') + "__warped_img.png",
            applyTransformation(tgtImage, shiftMat * tgtTfmMat, warpedImgSize)
        );

        if (i == 1) {
            cv::imwrite(orgIhmFolder + "00.JPG", srcImage);
            cv::imwrite(
                warpedFolder + "00__warped_img.png", 
                applyTransformation(srcImage, shiftMat * srcTfmMat, warpedImgSize)
            );
        }
    }

    std::cout << "Finish.." << std::endl;

    return 0;
}
