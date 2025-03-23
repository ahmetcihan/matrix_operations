#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("MFA501 Assessment 2A");
    // Buton bağlantılarını yap
    connect(ui->createMatrixButton, SIGNAL(clicked(bool)), this, SLOT(on_createMatrixButton_clicked()));
    connect(ui->createZeroMatrix, SIGNAL(clicked(bool)), this, SLOT(on_createZeroMatrix_clicked()));
    connect(ui->createUnitMatrix, SIGNAL(clicked(bool)), this, SLOT(on_createUnitMatrix_clicked()));

}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::on_createUnitMatrix_clicked()
{
    if (!matrixWidget) {
        qDebug() << "buraya girdi 1";
        return;
    }
    qDebug() << "buraya girdi 2";
    int rowCount = matrixWidget->rowCount();
    int colCount = matrixWidget->columnCount();

    qDebug() << "row" << rowCount << ", column" << colCount;

    for (int i = 0; i < rowCount; i++) {
        for (int j = 0; j < colCount; j++) {
            if(i == j){
                matrixWidget->item(i, j)->setText("1");
            }
            else{
                matrixWidget->item(i, j)->setText("0");
            }
            qDebug() << "i" << i << ", j" << j;
        }
    }
}
void MainWindow::on_createZeroMatrix_clicked()
{
    if (matrixWidget) {
        delete matrixWidget;  // Önce eski widget'ı sil
    }
    int n = ui->spinBox->value(); // Kullanıcının girdiği matris boyutu

    int rows = n;
    int cols = n;
    matrixWidget = new QTableWidget(rows, cols, this);
    matrixWidget->setEditTriggers(QAbstractItemView::AllEditTriggers);
    if(n < 8){
        matrixWidget->setFixedSize(50 * n + 40, 50 * n + 40); // Matrisin boyutunu dinamik ayarla
    }
    else{
        matrixWidget->setFixedSize(390, 390); // Matrisin boyutunu dinamik ayarla
    }

    for (int i = 0; i < n; i++) {
            matrixWidget->setColumnWidth(i, 50); // Sütun genişliği 50 piksel
            matrixWidget->setRowHeight(i, 50);   // Satır yüksekliği 50 piksel
    }
    matrixWidget->setParent(this);
    matrixWidget->move(20, 150); // X=50, Y=100 konumuna yerleştir
    matrixWidget->show();
    // Hücreleri oluştur
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrixWidget->setItem(i, j, new QTableWidgetItem("0")); // Boş hücre oluştur
        }
    }
}


void MainWindow::on_createMatrixButton_clicked()
{
    int n = ui->spinBox->value(); // Kullanıcının girdiği matris boyutu

    // Önceki tabloyu temizle
    if (matrixWidget) {
        delete matrixWidget;
        matrixWidget = nullptr;
    }

    // Yeni QTableWidget oluştur
    matrixWidget = new QTableWidget(n, n, this);
    matrixWidget->setEditTriggers(QAbstractItemView::AllEditTriggers);
    if(n < 8){
        matrixWidget->setFixedSize(50 * n + 40, 50 * n + 40); // Matrisin boyutunu dinamik ayarla
    }
    else{
        matrixWidget->setFixedSize(390, 390); // Matrisin boyutunu dinamik ayarla
    }

    for (int i = 0; i < n; i++) {
            matrixWidget->setColumnWidth(i, 50); // Sütun genişliği 50 piksel
            matrixWidget->setRowHeight(i, 50);   // Satır yüksekliği 50 piksel
    }
    // Ana pencerenin içine belirli bir konuma koy
    matrixWidget->setParent(this);
    matrixWidget->move(20, 150); // X=50, Y=100 konumuna yerleştir
    matrixWidget->show();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrixWidget->setItem(i, j, new QTableWidgetItem(" ")); // Boş hücre oluştur
        }
    }
}
