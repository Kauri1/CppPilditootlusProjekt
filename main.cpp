#include <opencv2/opencv.hpp>
#include <QApplication>
#include <QPushButton>
#include <QInputDialog>
#include <QMessageBox>

using namespace cv;

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    // Prompt the user for a string
    bool ok;
    QString text = QInputDialog::getText(nullptr, "Input Dialog",
                                         "Enter your name:", QLineEdit::Normal,
                                         "", &ok);

    if (ok && !text.isEmpty()) {
        QMessageBox::information(nullptr, "Input Received", "You entered: " + text);
    }

    //../Screenshot 2025-04-23 155822.png
    Mat image = imread(text.toStdString());
    if(image.empty()) {
        printf("Image not loaded!\n");
    } else {
        imshow("Display window", image);
        waitKey(0);
    }

    return 0;
}