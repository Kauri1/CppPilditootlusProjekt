#include <opencv2/opencv.hpp>
#include <QApplication>
#include <QPushButton>

using namespace cv;

int main(int argc, char *argv[]) {
    Mat image = imread("../Screenshot 2025-04-23 155822.png");
    if(image.empty()) {
        printf("Image not loaded!\n");
        return -1;
    }
    imshow("Display window", image);
    waitKey(0);


    QApplication a(argc, argv);
    QPushButton button("Hello world!", nullptr);
    button.resize(200, 100);
    button.show();
    return QApplication::exec();
}