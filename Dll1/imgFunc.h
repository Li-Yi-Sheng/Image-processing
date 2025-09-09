#pragma once

#ifdef IMGFUNC_EXPORTS
#define IMGFUNC_API __declspec(dllexport)  // 在編譯 DLL 時使用 dllexport
#else
#define IMGFUNC_API __declspec(dllexport)  // 在使用 DLL 時使用 dllimport
#endif

#include <opencv2/opencv.hpp>
#include <string>

extern "C" {
    IMGFUNC_API const char* calculateImageHash(const char* imagePath);
    IMGFUNC_API const char* extractWatermark(const char* imagePath, int len);
    IMGFUNC_API void embedWatermark(const char* imagePath, const char* watermarkText);

    IMGFUNC_API void applyNegative(const char* imagePath, const char* outputPath);
    IMGFUNC_API void applyBlur(const char* imagePath, const char* outputPath);
    IMGFUNC_API void applyEdgeDetection(const char* imagePath, const char* outputPath);
    IMGFUNC_API void applyHistogramEqualization(const char* imagePath, const char* outputPath);
    IMGFUNC_API void applySaltPepperNoise(const char* imagePath, const char* outputPath, float noiseRatio);
    IMGFUNC_API void applyGrayscale(const char* imagePath, const char* outputPath);
    IMGFUNC_API void applySharpen(const char* imagePath, const char* outputPath);
    IMGFUNC_API void applyContrastEnhancement(const char* imagePath, const char* outputPath);

}
