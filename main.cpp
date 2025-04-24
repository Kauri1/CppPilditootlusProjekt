#include <opencv2/opencv.hpp>
using namespace cv;

int main() {
    Mat image = imread("../Screenshot 2025-04-23 155822.png");
    if(image.empty()) {
        printf("Image not loaded!\n");
        return -1;
    }
    imshow("Display window", image);
    waitKey(0);
    return 0;
}