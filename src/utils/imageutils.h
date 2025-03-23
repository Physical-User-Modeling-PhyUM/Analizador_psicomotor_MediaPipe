#ifndef IMAGEUTILS_H
#define IMAGEUTILS_H

#include <opencv2/opencv.hpp>

class ImageUtils
{
public:
    static bool areSameImage(const cv::Mat& mat1, const cv::Mat& mat2);
};

#endif // IMAGEUTILS_H
