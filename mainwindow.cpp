#include "mainwindow.hpp"
#include <QMimeData>
#include <QUrl>
#include <QVBoxLayout>
#include <QMessageBox>
#include <opencv2/opencv.hpp>
#include <QImage>
#include <QPixmap>
#include <QListWidgetItem>
#include <QFileInfo>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setAcceptDrops(true);
    resize(600, 400); // dropboxi mõõdud

    listWidget = new QListWidget(this);
    listWidget->setViewMode(QListView::IconMode);  //Näita pildid ikoonidena
    listWidget->setIconSize(QSize(128, 128));      //preview ikooni suurus
    listWidget->setResizeMode(QListView::Adjust);
    listWidget->setMovement(QListView::Static);
    setCentralWidget(listWidget);

    // teeb pildi pathi "clickitavaks" et uuesti avada
    connect(listWidget, &QListWidget::itemDoubleClicked,
            this, &MainWindow::onItemDoubleClicked);
}


MainWindow::~MainWindow() = default;

void MainWindow::dragEnterEvent(QDragEnterEvent *event) {
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent *event) {
    const QList<QUrl> urls = event->mimeData()->urls();
    for (const QUrl &url : urls) {
        QString filePath = url.toLocalFile();

        // Proovib laadida pildi eelvaateks
        cv::Mat img = cv::imread(filePath.toStdString());

        // Kui pole sobiv fail, siis teeb järgmist
        if (img.empty()) {
            QMessageBox::warning(this, "Import Error", "Fail " + filePath + "See ei ole toetatud failitüüp.");
        } else {

            // OpenCV pilt, muuda see QImage-ks
            QImage qimg(img.data, img.cols, img.rows, img.step, QImage::Format_BGR888);
            QPixmap pixmap = QPixmap::fromImage(qimg.scaled(128, 128, Qt::KeepAspectRatio, Qt::SmoothTransformation));

            QListWidgetItem *item = new QListWidgetItem(QIcon(pixmap), QFileInfo(filePath).fileName(), listWidget);
            item->setData(Qt::UserRole, filePath);  // Salvestab originaali tee varjatud andmena
            listWidget->addItem(item);

            showImage(filePath);  // Näita kohe suurt pilti (kui see on pilt)
        }


    }
}

void MainWindow::showImage(const QString &filePath) {
    cv::Mat image = cv::imread(filePath.toStdString());
    if (image.empty()) {
        QMessageBox::warning(this, "Load Error", "See pole pilt");
        return;
    }
    cv::namedWindow("Image Preview", cv::WINDOW_AUTOSIZE);
    cv::imshow("Image Preview", image);
    cv::waitKey(1); // waitKey(0) peataks GUI, seega kasuta väiksemat väärtust
}

// teeb pildi pathi "clickitavaks" et uuesti avada
void MainWindow::onItemDoubleClicked(QListWidgetItem *item) {
    QVariant data = item->data(Qt::UserRole);
    if (data.isValid()) {
        QString filePath = data.toString();
        showImage(filePath);
    }
    else {
        QMessageBox::information(this, "Info", "See element ei ole pildifail.");
    }
}
