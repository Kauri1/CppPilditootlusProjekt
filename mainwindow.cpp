#include "mainwindow.hpp"
#include "ImageProcessor.hpp"

#include <QMimeData>           // Drag & drop
#include <QUrl>
#include <QVBoxLayout>         // Vertikaalne paigutus
#include <QHBoxLayout>         // Horisontaalne paigutus
#include <QMessageBox>
#include <QImage>
#include <QPixmap>             // Pildi kuvamine
#include <QFileInfo>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>



// MainWindow konstruktor
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setAcceptDrops(true); // Lubab aknasse faile lohistada
    resize(600, 400);     // Dropboxi mõõdud
    setupUI();            // Ehita kasutajaliides
}

// Kasutajaliidese loomine
void MainWindow::setupUI() { //loob selle liidese UI

    // Loo main widget ja paigutus
    auto *centralWidget = new QWidget(this);
    auto *mainLayout = new QHBoxLayout(centralWidget);


    // Loo pildiliste failide nimekiri (ikoon ja nimi selle kõrval)
    listWidget = new QListWidget(this);
    listWidget->setViewMode(QListView::ListMode);
    listWidget->setIconSize(QSize(64, 64));        // või 128x128 kui suured ikoonid
    listWidget->setResizeMode(QListView::Adjust);
    listWidget->setMovement(QListView::Static);
    listWidget->setSpacing(5);                     // veidi ruumi ikoonide vahele
    listWidget->setMinimumWidth(200);
    listWidget->setMaximumWidth(300);


    /*
    // Loo pildiliste failide nimekiri (ikoon ja nimi selle all)
    listWidget = new QListWidget(this);
    listWidget->setViewMode(QListView::IconMode);
    listWidget->setIconSize(QSize(128, 128));
    listWidget->setResizeMode(QListView::Adjust);
    listWidget->setMovement(QListView::Static);
    listWidget->setMinimumWidth(200);
    listWidget->setMaximumWidth(300);
    */

    // Loo trackbaride paneel
    controlsWidget = new QWidget(this);
    auto *controlsLayout = new QVBoxLayout(controlsWidget);


    // Loo trackbarid
    blurSlider = new QSlider(Qt::Horizontal);
    brightnessSlider = new QSlider(Qt::Horizontal);
    contrastSlider = new QSlider(Qt::Horizontal);
    saturationSlider = new QSlider(Qt::Horizontal);
    edgeDetectionSlider = new QSlider(Qt::Horizontal);


    // Configureeri trackbarid (väärtusvahemikud)
    blurSlider->setRange(0, 20);  // kernel suuruses 1-21
    brightnessSlider->setRange(-254, 254);
    contrastSlider->setRange(0, 200);  // 0-2 kordne kontrast
    saturationSlider->setRange(0, 200);  // 0-2 kordne saturation
    edgeDetectionSlider->setRange(0, 4);


    // Vaikeväärtused
    blurSlider->setValue(0);
    brightnessSlider->setValue(0);
    contrastSlider->setValue(100);  // 100 on normaalne kontrast
    saturationSlider->setValue(100);  // 100 on normaalne saturation
    edgeDetectionSlider->setValue(0);


    // Loo nimed trackbaride jaoks
    brightnessLabel = new QLabel("Brightness:", this);
    contrastLabel = new QLabel("Contrast:", this);
    saturationLabel = new QLabel("Saturation:", this);
    blurLabel = new QLabel("Blur:", this);
    edgeLabel = new QLabel("Edge Detection:", this);

    // Loo reset button
    resetButton = new QPushButton("Reset All", this);

    savePath = new QLineEdit(this);

    saveButton = new QPushButton("Save", this);

    // Lisa widgetid controls layoutile
    controlsLayout->addWidget(brightnessLabel);
    controlsLayout->addWidget(brightnessSlider);
    controlsLayout->addWidget(contrastLabel);
    controlsLayout->addWidget(contrastSlider);
    controlsLayout->addWidget(saturationLabel);
    controlsLayout->addWidget(saturationSlider);
    controlsLayout->addWidget(blurLabel);
    controlsLayout->addWidget(blurSlider);
    controlsLayout->addWidget(edgeLabel);
    controlsLayout->addWidget(edgeDetectionSlider);
    controlsLayout->addWidget(resetButton);
    controlsLayout->addWidget(savePath);
    controlsLayout->addWidget(saveButton);

    controlsLayout->addStretch(); // Venitab tühja ruumi alla

    // Lisa widgetid main layoutile
    mainLayout->addWidget(listWidget);
    mainLayout->addWidget(controlsWidget);

    setCentralWidget(centralWidget);

    // Ühenda trackbarid ja nupp vastavate funktsioonidega.
    connect(blurSlider, &QSlider::valueChanged, this, &MainWindow::onBlurValueChanged);
    connect(brightnessSlider, &QSlider::valueChanged, this, &MainWindow::onBrightnessValueChanged);
    connect(contrastSlider, &QSlider::valueChanged, this, &MainWindow::onContrastValueChanged);
    connect(saturationSlider, &QSlider::valueChanged, this, &MainWindow::onSaturationValueChanged);
    connect(edgeDetectionSlider, &QSlider::valueChanged, this, &MainWindow::onEdgeDetectionValueChanged);
    connect(resetButton, &QPushButton::clicked, this, &MainWindow::resetAdjustments);
    connect(listWidget, &QListWidget::itemDoubleClicked, this, &MainWindow::onItemDoubleClicked);
    connect(saveButton, &QPushButton::clicked, this, &MainWindow::saveImage);
    connect(savePath, &QLineEdit::returnPressed, this, &MainWindow::saveImage);
}




MainWindow::~MainWindow() = default;

// Lohistamise sisse lubamine
void MainWindow::dragEnterEvent(QDragEnterEvent *event) {
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    }
}

// Failide "kukutamine" aknasse
void MainWindow::dropEvent(QDropEvent *event) {
    const QList<QUrl> urls = event->mimeData()->urls();
    for (const QUrl &url : urls) {
        QString filePath = url.toLocalFile();

        // Proovib laadida pildi eelvaateks
        cv::Mat img = cv::imread(filePath.toStdString());

        // Kui fail ei ole pilt, siis näita veateadet
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

// Näitab valitud pilti
void MainWindow::showImage(const QString &filePath) {
    currentImagePath = filePath;
    originalImage = cv::imread(filePath.toStdString());
    if (originalImage.empty()) {
        QMessageBox::warning(this, "Load Error", "Cannot load image");
        return;
    }
    processedImage = originalImage.clone();
    updateImage();
}

// Uuendab pildi eelvaadet
void MainWindow::updateImage() {
    if (processedImage.empty()) return;

    cv::Mat result = originalImage.clone();

    // Rakenda efektid
    if (blurSlider->value() > 0) {
        int kernelSize = 1 + (2 * blurSlider->value());  // Kernel peab olema paaritu arv
        result = gBlur(result, kernelSize);
    }

    if (brightnessSlider->value() != 0) {
        result = Brighten(result, brightnessSlider->value());
    }

    if (contrastSlider->value() != 100) {  // 100 tähendab normaalset kontrasti
        result = Contrast(result, contrastSlider->value() / 100.0f);
    }

    if (saturationSlider->value() != 100) {  // 100 tähendab normaalset satureeritust
        result = Saturate(result, saturationSlider->value() / 100.0f);
    }

    if (edgeDetectionSlider->value() != 0) {
        result = EdgeDetect(result, edgeDetectionSlider->value());
    }

    cv::imshow("Image Preview", result);
    processedImage = result;
    cv::waitKey(1);
}


void MainWindow::onBlurValueChanged(int) {
    updateImage();
}

void MainWindow::onBrightnessValueChanged(int) {
    updateImage();
}

void MainWindow::onContrastValueChanged(int) {
    updateImage();
}

void MainWindow::onSaturationValueChanged(int) {
    updateImage();
}

void MainWindow::onEdgeDetectionValueChanged(int) {
    updateImage();
}

void MainWindow::resetAdjustments() {
    blurSlider->setValue(0);
    brightnessSlider->setValue(0);
    contrastSlider->setValue(100);  // Reseti 100-le
    saturationSlider->setValue(100);  // Reseti 100-le
    edgeDetectionSlider->setValue(0);

    if (!originalImage.empty()) {
        processedImage = originalImage.clone();
        updateImage();
    }
}

void MainWindow::saveImage() {
    if (currentImagePath.isEmpty()) {
        QMessageBox::warning(this, "Save Error", "No image to save");
        return;
    }
    cv::imwrite(savePath->text().toStdString(), processedImage);
    QMessageBox::information(this, "Save Success", "Image saved successfully");
}




// teeb pildi pathi "clickitavaks" et uuesti avada
void MainWindow::onItemDoubleClicked(QListWidgetItem *item) {
    QVariant data = item->data(Qt::UserRole);
    if (data.isValid()) {
        QString filePath = data.toString();
        showImage(filePath); // Ava pilt
    }
    else {
        QMessageBox::information(this, "Info", "See element ei ole pildifail.");
    }
}
