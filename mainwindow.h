#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_createMatrixButton_clicked(); // Buton tıklanınca çalışacak fonksiyon
    void on_createZeroMatrix_clicked();
    void on_createUnitMatrix_clicked();

private:
    Ui::MainWindow *ui;
    QTableWidget *matrixWidget = nullptr; // Dinamik matris widget'ı
};
#endif // MAINWINDOW_H
