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
#include <QCheckBox>



// MainWindow konstruktor
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setAcceptDrops(true);    // Lubab aknasse faile lohistada
    resize(300, 800);     // Dropboxi vaikeväärtuse mõõdud
    setupUI();                  // Ehita kasutajaliides
    showMaximized();            // Avab controlsLayout akna maximized olekus
}

// Kasutajaliidese loomine
void MainWindow::setupUI() { //loob selle liidese UI

    // Loo main widget ja paigutus
    auto *centralWidget = new QWidget(this);
    auto *mainLayout = new QHBoxLayout(centralWidget);



    //TRACKBARID

    // Loo trackbaride aken
    controlsWidget = new QWidget(this);
    auto *controlsLayout = new QVBoxLayout(controlsWidget);

    // Loo trackbarid
    blurSlider = new QSlider(Qt::Horizontal);
    brightnessSlider = new QSlider(Qt::Horizontal);
    contrastSlider = new QSlider(Qt::Horizontal);
    saturationSlider = new QSlider(Qt::Horizontal);
    edgeDetectionSlider = new QSlider(Qt::Horizontal);

    // Trackbaride väärtusvahemikud
    blurSlider->setRange(0, 20);         // kernel suuruses 1-21
    brightnessSlider->setRange(-254, 254);
    contrastSlider->setRange(0, 200);    // 0-2 kordne kontrast
    saturationSlider->setRange(0, 200);  // 0-2 kordne saturation
    edgeDetectionSlider->setRange(0, 4); // kerneli suurus 0-4

    // Trackbaride mõõdud ja omadused
    int fixedWidth = 300; // trackbarid on sama laiad kui piltide list
    blurSlider->setFixedWidth(fixedWidth);
    brightnessSlider->setFixedWidth(fixedWidth);
    contrastSlider->setFixedWidth(fixedWidth);
    saturationSlider->setFixedWidth(fixedWidth);
    edgeDetectionSlider->setFixedWidth(fixedWidth);

    // Vaikeväärtused
    blurSlider->setValue(0);
    brightnessSlider->setValue(0);
    contrastSlider->setValue(100);    // 100 on normaalne kontrast
    saturationSlider->setValue(100);  // 100 on normaalne saturation
    edgeDetectionSlider->setValue(0);

    // Loo nimed trackbaride jaoks
    brightnessLabel = new QLabel("Brightness:", this);
    contrastLabel = new QLabel("Contrast:", this);
    saturationLabel = new QLabel("Saturation:", this);
    blurLabel = new QLabel("Blur:", this);
    edgeLabel = new QLabel("Edge Detection:", this);


    // NUPUD

    resetButton = new QPushButton("Reset All", this);
    savePath = new QLineEdit(this);
    saveButton = new QPushButton("Save", this);
    showOriginalImgButton = new QPushButton("Show Original Image", this);

    // Nuppude laiused
    resetButton->setFixedWidth(fixedWidth);
    saveButton->setFixedWidth(fixedWidth);
    savePath->setFixedWidth(fixedWidth);
    showOriginalImgButton->setFixedWidth(125);

    // CHECKBOX

    // checkboxide kõrvuti paigutamine
    auto *checkboxLayout = new QHBoxLayout();
    controlsLayout->addLayout(checkboxLayout);

    openInNewWindowCheckBox = new QCheckBox("Open in New Window", this);
    checkboxLayout->addWidget(openInNewWindowCheckBox);

    //showOriginalImgCheckbox = new QCheckBox("Show Original Image", this);
    checkboxLayout->addWidget(showOriginalImgButton);



    // PILTIDE LIST

    // Loo list piltidest, mis on dropboxi lohistatud (salvestab ikooni ja nime selle kõrvale)
    listWidget = new QListWidget(this);
    listWidget->setViewMode(QListView::ListMode);
    listWidget->setIconSize(QSize(64, 64));        // või 128x128 kui suured ikoonid
    listWidget->setResizeMode(QListView::Adjust);
    listWidget->setMovement(QListView::Static);
    listWidget->setSpacing(5);                     // veidi ruumi ikoonide vahele
    listWidget->setFixedWidth(fixedWidth);               // listi fixed laius (fixedWidth on trackbaride juures)
    listWidget->setMinimumHeight(100);              // listi pikkus (akna suurusega muudetav)



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
    controlsLayout->addSpacing(5); //väike vahe slaiderite ja nuppude vahel

    controlsLayout->addWidget(resetButton);
    controlsLayout->addSpacing(5);
    controlsLayout->addWidget(savePath);
    controlsLayout->addWidget(saveButton);
    controlsLayout->addSpacing(5); //vahe nuppude ja listi vahel


    // Paiguta piltide list trackbaride alla
    controlsLayout->addWidget(listWidget);
    //controlsLayout->addStretch(); // Tekitab tühja ruumi listi alla


    // PILT SEADETE AKNAS
    imageLabel = new QLabel(this);
    imageLabel->setFixedSize(935, 615); // Pildi kuvamise ala suurus (fixed size)
    imageLabel->setAlignment(Qt::AlignCenter); // Pilt paigutatakse keksele
    //imageLabel->setStyleSheet("border: 1px solid black;"); // Border


    // Lisa widgetid main layoutile
    mainLayout->addWidget(controlsWidget);  // trackbarid ja list
    mainLayout->addWidget(imageLabel);      // pildi nimi
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
    connect(openInNewWindowCheckBox, &QCheckBox::toggled, this, &MainWindow::onCheckboxToggled);
    //connect(showOriginalImgCheckbox, &QCheckBox::toggled, this, &MainWindow::onShowOriginalImageToggled);
    connect(showOriginalImgButton, &QPushButton::clicked, this, &MainWindow::onShowOriginalImageClicked);

    //qDebug() << "Setup UI completed"; //debug
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

// Uuendab pilti aknal
void MainWindow::updateImage() {
    if (processedImage.empty()) return;

    cv::Mat result = originalImage.clone();

    // Lisa efektid
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

    // Kontrolli kas checkbox on vajutatud
    if (openInNewWindowCheckBox->isChecked()) {
        cv::imshow("Image Preview", result);
        cv::waitKey(1);
    }

    // Muuda QPixmap pildiks ja kuva main aknas
    QImage qimg(result.data, result.cols, result.rows, result.step, QImage::Format_BGR888);
    QPixmap pixmap = QPixmap::fromImage(qimg.scaled(imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    imageLabel->setPixmap(pixmap);


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

void MainWindow::onCheckboxToggled(bool checked) {
    if (checked) {
        // Kui checkbox vajutatud, teeb pildi jaoks teise akna
        if (!processedImage.empty()) {
            cv::imshow("Image Preview", processedImage);
            isPreviewWindowOpen = true;
            cv::waitKey(1);
        }
    } else {
        // Kui pole vajutatud hävitab akna
        if (isPreviewWindowOpen == true) {
            cv::destroyWindow("Image Preview");
            isPreviewWindowOpen = false;
        }
    }
}

/*
void MainWindow::onShowOriginalImageToggled(bool checked) {
    if (checked) {
        // kui vajutatud, siis näitab töödeldud pildi asemel originaalset (ainult main aknal)
        if (!originalImage.empty()) {
            QImage qimg(originalImage.data, originalImage.cols, originalImage.rows, originalImage.step, QImage::Format_BGR888);
            QPixmap pixmap = QPixmap::fromImage(qimg.scaled(imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
            imageLabel->setPixmap(pixmap);
            isShowingOriginal = true;
        }
    }
    else {
        // uuendab akent, et checkboxi vajutades muudatus ise juhtuks
        updateImage();
        isShowingOriginal = false;
    }
    // teine aken alati näitab trackbaride muudatusi
    if (openInNewWindowCheckBox->isChecked() && !processedImage.empty()) {
        cv::imshow("Image Preview", processedImage);
        cv::waitKey(1);
    }
}
*/

void MainWindow::onShowOriginalImageClicked() {
    // Show the original image in the main window
    if (!originalImage.empty()) {
        QImage qimg(originalImage.data, originalImage.cols, originalImage.rows, originalImage.step, QImage::Format_BGR888);
        QPixmap pixmap = QPixmap::fromImage(qimg.scaled(imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        imageLabel->setPixmap(pixmap);
    }
}