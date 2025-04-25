#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QListWidget>
#include <QDragEnterEvent>
#include <QDropEvent>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private:
    QListWidget *listWidget;
    void showImage(const QString &filePath);


private slots:
    // teeb pildi pathi "clickitavaks" et uuesti avada
    void onItemDoubleClicked(QListWidgetItem *item);
};

#endif // MAINWINDOW_HPP