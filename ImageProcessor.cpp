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

/*cv::Mat Contrast(const cv::Mat &img, float amount) {
    cv::Mat contrasted = img.clone();
    // amount should be > 0, where:
    // amount < 1 decreases contrast
    // amount > 1 increases contrast

    for (int i = 0; i < img.rows; i++) {
        for (int j = 0; j < img.cols; j++) {
            cv::Vec3b pixel = img.at<cv::Vec3b>(i, j);
            for (int k = 0; k < 3; k++) {
                // Normalize to [0,1] range
                float normalized = (pixel[k] / 127.5f);
                // Apply power function for contrast
                float adjusted = std::pow(normalized, amount);
                // Convert back to [0,255] range
                contrasted.at<cv::Vec3b>(i, j)[k] = cv::saturate_cast<uchar>(adjusted * 127.5f);
            }
        }
    }
    return contrasted;
}*/

cv::Mat Contrast(const cv::Mat &img, float amount) {
    cv::Mat contrasted(img.size(), img.type());

    img.convertTo(contrasted, -1, std::pow(amount,1.5), (1.0f - std::pow(amount,1.5)) * 127); // shifts midpoint to make blacks darker

    return contrasted;
}

cv::Mat WhitePoint(const cv::Mat &img, float amount) {
    cv::Mat contrasted(img.size(), img.type());
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

cv::Mat EdgeDetect(const cv::Mat &img, int kernelSize) {
    cv::Mat edge;
    //std::vector<int> kernel = {-1, -1, -1, -1, 8, -1, -1, -1, -1};
    std::vector<float> kernel((kernelSize*2+1) * (kernelSize*2+1), -1);
    //for (int i = 0; i < kernel.size(); i++) {
    //    kernel[i] = -1;
    //}
    kernel[kernelSize*2+1] = ((kernelSize*2+1)*(kernelSize*2+1))-1;
    cv::Mat kernelMat = cv::Mat((kernelSize*2+1), (kernelSize*2+1), CV_32F, kernel.data());
    cv::filter2D(img, edge, -1, kernelMat);
    return edge;
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


