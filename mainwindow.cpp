#include "mainwindow.hpp"
#include <QMimeData>
#include <QUrl>
#include <QVBoxLayout>
#include <QMessageBox>
#include <opencv2/opencv.hpp>

using namespace cv;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setAcceptDrops(true);
    resize(400, 300);

    listWidget = new QListWidget(this);
    setCentralWidget(listWidget);
}

MainWindow::~MainWindow() {}

void MainWindow::dragEnterEvent(QDragEnterEvent *event) {
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent *event) {
    const QList<QUrl> urls = event->mimeData()->urls();
    for (const QUrl &url : urls) {
        QString filePath = url.toLocalFile();
        listWidget->addItem(filePath);
        showImage(filePath);  // Näita pilti kui see on pilt
    }
}

void MainWindow::showImage(const QString &filePath) {
    Mat image = imread(filePath.toStdString());
    if (image.empty()) {
        QMessageBox::warning(this, "Load Error", "Image could not be loaded.");
        return;
    }
    namedWindow("Image Preview", WINDOW_AUTOSIZE);
    imshow("Image Preview", image);
    waitKey(1); // waitKey(0) peataks GUI, seega kasuta väiksemat väärtust
}
