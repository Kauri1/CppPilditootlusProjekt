#include "ImageProcessor.hpp"


cv::Mat gBlur(const cv::Mat &img, int kernelSize) {
    cv::Mat blurredImg;
    cv::GaussianBlur(img, blurredImg, cv::Size(kernelSize, kernelSize), 0); // Kernel suurusega 5x5
    return blurredImg;
}

cv::Mat Brighten(const cv::Mat &img, int amount) {
    cv::Mat brightened = img + cv::Scalar(amount, amount, amount); // lisab igale piksli igale värvile amount
    return brightened;
}

cv::Mat Contrast(const cv::Mat &img, float amount) {
    cv::Mat contrasted;
    img.convertTo(contrasted, -1, amount, 0); // contrasted(x,y) = alpha * img(x,y) + beta
    return contrasted;
}

cv::Mat Saturate(const cv::Mat &img, float amount) {
    cv::Mat hsv;
    cv::cvtColor(img, hsv, cv::COLOR_BGR2HSV);
    std::vector<cv::Mat> channels;
    cv::split(hsv, channels);
    channels[1] = channels[1] * amount;
    cv::Mat saturated;
    cv::merge(channels, hsv);
    cv::cvtColor(hsv, saturated, cv::COLOR_HSV2BGR);
    return saturated;
}




/*int main() {
    const cv::Mat img = cv::imread("../noit.png");
    const cv::Mat blurred = gBlur(img, 15);
    const cv::Mat brightened = Brighten(img, 100);
    const cv::Mat contrasted = Contrast(img, 2);
    const cv::Mat saturated = Saturate(img, 0); // et saada mustvalge
    cv::imshow("Original", img);
    cv::imshow("Blurred", blurred);
    cv::imshow("Brightened", brightened);
    cv::imshow("Contrasted", contrasted);
    cv::imshow("Saturated", saturated);
    cv::waitKey(0);

}*/


