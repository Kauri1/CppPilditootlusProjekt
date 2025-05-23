#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QListWidget>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMessageBox>
#include <opencv2/core/mat.hpp>
#include <QVBoxLayout>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
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

    void onEdgeDetectionValueChanged(int);

    void resetAdjustments();

    void saveImage();

    QSlider *blurSlider;
    QSlider *brightnessSlider;
    QSlider *contrastSlider;
    QSlider *saturationSlider;
    QSlider *edgeDetectionSlider;
    QPushButton *resetButton;
    QWidget *controlsWidget;
    QLineEdit *savePath;
    QPushButton *saveButton;

    QLabel *blurLabel;
    QLabel *brightnessLabel;
    QLabel *contrastLabel;
    QLabel *saturationLabel;
    QLabel *edgeLabel;

    QCheckBox *openInNewWindowCheckBox; // Checkbox uue akna avamiseks
    QVBoxLayout *controlsLayout;
    QLabel *imageLabel;                 // Pildi akna nimi

private slots:
    void onItemDoubleClicked(QListWidgetItem *item);     // teeb pildi pathi "clickitavaks" et uuesti avada
};



#endif // MAINWINDOW_HPP