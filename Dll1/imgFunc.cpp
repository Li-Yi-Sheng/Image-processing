#include "pch.h"
#include "imgFunc.h"
#include <opencv2/opencv.hpp>
#include <sstream>
#include <iomanip>
#include <bitset>
#include <cstdlib>

using namespace cv;

// 計算影像的簡單哈希值
IMGFUNC_API const char* calculateImageHash(const char* imagePath) {
    static std::string hexStr;
    std::stringstream ss;
    cv::Mat image = cv::imread(imagePath, cv::IMREAD_COLOR);
    for (int i = 0; i < image.rows; ++i) {
        for (int j = 0; j < image.cols; ++j) {
            cv::Vec3b pixel = image.at<cv::Vec3b>(i, j);
            ss << static_cast<int>(pixel[0]) << static_cast<int>(pixel[1]) << static_cast<int>(pixel[2]);
        }
    }

    std::string imageData = ss.str();
    std::hash<std::string> hasher;
    size_t hashValue = hasher(imageData);

    std::stringstream hexStream;
    hexStream << std::hex << hashValue;
    hexStr = hexStream.str();
    return hexStr.c_str();
}

// 從影像中提取隱形水印
IMGFUNC_API const char* extractWatermark(const char* imagePath, int len) {
    std::string watermark;
    static std::string asciiStr = "";
    asciiStr.clear();
    watermark.clear();
    cv::Mat image = cv::imread(imagePath, cv::IMREAD_COLOR);
    size_t watermarkSize = len;
    size_t bitIndex = 0;

    for (int i = 0; i < image.rows && bitIndex < watermarkSize * 8; ++i) {
        for (int j = 0; j < image.cols && bitIndex < watermarkSize * 8; ++j) {
            cv::Vec3b pixel = image.at<cv::Vec3b>(i, j);
            for (int c = 0; c < 3; ++c) {
                if (bitIndex < watermarkSize * 8) {
                    char bit = pixel[c] & 1;
                    watermark.push_back(bit ? '1' : '0');
                    ++bitIndex;
                }
            }
        }
    }

    int len2 = watermark.length();
    if (len2 % 8 != 0) {
        return "";
    }

    for (int i = 0; i < len2; i += 8) {
        std::string byte = watermark.substr(i, 8);
        char character = static_cast<char>(std::bitset<8>(byte).to_ulong());
        asciiStr += character;
    }

    return asciiStr.c_str();
}

// 嵌入隱形水印
IMGFUNC_API void embedWatermark(const char* imagePath, const char* watermarkText) {
    size_t textLength = strlen(watermarkText);
    size_t bitIndex = 0;

    cv::Mat image = cv::imread(imagePath, cv::IMREAD_COLOR);
    if (image.empty()) return;

    size_t availableBits = image.rows * image.cols * 3;
    size_t watermarkBits = textLength * 8;

    if (watermarkBits > availableBits) return;

    for (int i = 0; i < image.rows && bitIndex < watermarkBits; ++i) {
        for (int j = 0; j < image.cols && bitIndex < watermarkBits; ++j) {
            cv::Vec3b& pixel = image.at<cv::Vec3b>(i, j);
            for (int c = 0; c < 3 && bitIndex < watermarkBits; ++c) {
                char bit = (watermarkText[bitIndex / 8] >> (7 - (bitIndex % 8))) & 1;
                pixel[c] = (pixel[c] & 0xFE) | bit;
                ++bitIndex;
            }
        }
    }

    const char* lastSlash = strrchr(imagePath, '\\');
    std::string folderPath(imagePath, lastSlash + 1);
    std::string outputPath = folderPath + "hash.png";
    cv::imwrite(outputPath, image);
}

// 負片效果
IMGFUNC_API void applyNegative(const char* imagePath, const char* outputPath) {
    cv::Mat image = cv::imread(imagePath, cv::IMREAD_COLOR);
    if (image.empty()) return;

    cv::Mat negative;
    cv::bitwise_not(image, negative);
    cv::imwrite(outputPath, negative);
}

// 模糊效果
IMGFUNC_API void applyBlur(const char* imagePath, const char* outputPath) {
    cv::Mat image = cv::imread(imagePath, cv::IMREAD_COLOR);
    if (image.empty()) return;

    cv::Mat blurred;
    cv::GaussianBlur(image, blurred, cv::Size(35, 35), 0);
    cv::imwrite(outputPath, blurred);
}

// 邊緣檢測
IMGFUNC_API void applyEdgeDetection(const char* imagePath, const char* outputPath) {
    cv::Mat image = cv::imread(imagePath, cv::IMREAD_COLOR);
    if (image.empty()) return;

    cv::Mat grayscale, edges;
    cv::cvtColor(image, grayscale, cv::COLOR_BGR2GRAY);
    cv::Canny(grayscale, edges, 100, 200); // 設定閾值 100 與 200
    cv::imwrite(outputPath, edges);
}

// 直方圖均衡化
IMGFUNC_API void applyHistogramEqualization(const char* imagePath, const char* outputPath) {
    cv::Mat image = cv::imread(imagePath, cv::IMREAD_COLOR);
    if (image.empty()) return;

    cv::Mat grayscale, equalized;
    cv::cvtColor(image, grayscale, cv::COLOR_BGR2GRAY);
    cv::equalizeHist(grayscale, equalized);
    cv::imwrite(outputPath, equalized);
}

// 添加胡椒鹽雜訊
IMGFUNC_API void applySaltPepperNoise(const char* imagePath, const char* outputPath, float noiseRatio) {
    cv::Mat image = cv::imread(imagePath, cv::IMREAD_COLOR);
    if (image.empty()) return;

    cv::Mat noisyImage = image.clone();
    int totalPixels = noisyImage.rows * noisyImage.cols;
    int noisePixels = static_cast<int>(totalPixels * noiseRatio);

    for (int i = 0; i < noisePixels; ++i) {
        int x = rand() % noisyImage.cols;
        int y = rand() % noisyImage.rows;

        if (rand() % 2) {
            noisyImage.at<cv::Vec3b>(y, x) = cv::Vec3b(255, 255, 255);
        }
        else {
            noisyImage.at<cv::Vec3b>(y, x) = cv::Vec3b(0, 0, 0);
        }
    }

    cv::imwrite(outputPath, noisyImage);
}
// 灰階轉換
IMGFUNC_API void applyGrayscale(const char* imagePath, const char* outputPath) {
    cv::Mat image = cv::imread(imagePath, cv::IMREAD_COLOR);
    if (image.empty()) return;

    cv::Mat grayscale;
    cv::cvtColor(image, grayscale, cv::COLOR_BGR2GRAY);
    cv::imwrite(outputPath, grayscale);
}
// 銳化處理
IMGFUNC_API void applySharpen(const char* imagePath, const char* outputPath) {
    cv::Mat image = cv::imread(imagePath, cv::IMREAD_COLOR);
    if (image.empty()) return;

    cv::Mat sharpened, kernel;
    kernel = (cv::Mat_<float>(3, 3) <<
        0, -1, 0,
        -1, 9, -1,
        0, -1, 0);
    cv::filter2D(image, sharpened, image.depth(), kernel);
    cv::imwrite(outputPath, sharpened);
}
// 對比度增強
IMGFUNC_API void applyContrastEnhancement(const char* imagePath, const char* outputPath) {
    cv::Mat image = cv::imread(imagePath, cv::IMREAD_COLOR);
    if (image.empty()) return;

    cv::Mat enhanced;
    image.convertTo(enhanced, -1, 1.5, 0); // 1.5 是對比增強的倍數
    cv::imwrite(outputPath, enhanced);
}
