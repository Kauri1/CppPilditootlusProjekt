#ifndef IMAGEPROCESSOR_HPP
#define IMAGEPROCESSOR_HPP

#include <opencv2/opencv.hpp>

cv::Mat gBlur(const cv::Mat &img, int kernelSize);
cv::Mat Brighten(const cv::Mat &img, int amount);
cv::Mat Contrast(const cv::Mat &img, float amount);
cv::Mat Saturate(const cv::Mat &img, float amount);


#endif //IMAGEPROCESSOR_HPP
