#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QListWidget>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMessageBox>
#include <opencv2/core/mat.hpp>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);



    ~MainWindow();

protected:
    void setupUI();

    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private:
    QListWidget *listWidget;
    QString currentImagePath;
    cv::Mat originalImage;
    cv::Mat processedImage;


    void showImage(const QString &filePath);

    void updateImage();

    void onBlurValueChanged(int);

    void onBrightnessValueChanged(int);

    void onContrastValueChanged(int);

    void onSaturationValueChanged(int);

    void resetAdjustments();

    QSlider *blurSlider;
    QSlider *brightnessSlider;
    QSlider *contrastSlider;
    QSlider *saturationSlider;
    QPushButton *resetButton;
    QWidget *controlsWidget;

    QLabel *blurLabel;
    QLabel *brightnessLabel;
    QLabel *contrastLabel;
    QLabel *saturationLabel;



private slots:
    // teeb pildi pathi "clickitavaks" et uuesti avada
    void onItemDoubleClicked(QListWidgetItem *item);
};



#endif // MAINWINDOW_HPP