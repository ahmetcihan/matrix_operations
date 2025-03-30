#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("MFA501 Assessment 2A - Ahmet Cihan AKINCA");

    connect(ui->createMatrixButton, SIGNAL(clicked(bool)), this, SLOT(on_createMatrixButton_clicked()));
    connect(ui->createZeroMatrix, SIGNAL(clicked(bool)), this, SLOT(on_createZeroMatrix_clicked()));
    connect(ui->createUnitMatrix, SIGNAL(clicked(bool)), this, SLOT(on_createUnitMatrix_clicked()));
    connect(ui->createUpperTriangularMatrix, SIGNAL(clicked(bool)), this, SLOT(on_createUpperTriangularMatrix_clicked()));
    connect(ui->createLowerTriangularMatrix, SIGNAL(clicked(bool)), this, SLOT(on_createLowerTriangularMatrix_clicked()));
    connect(ui->createRandomMatrix, SIGNAL(clicked(bool)), this, SLOT(on_createRandomMatrix_clicked()));
    connect(ui->calculateDeterminant, SIGNAL(clicked(bool)), this, SLOT(on_calculateDeterminant_clicked()));
    connect(ui->calculateDeterminant, SIGNAL(clicked(bool)), this, SLOT(calculateDeterminant()));
    connect(ui->calculateEigenvalues, SIGNAL(clicked(bool)), this, SLOT(on_calculateEigenvalues_clicked()));
    connect(ui->calculateEigenvectors, SIGNAL(clicked(bool)), this, SLOT(on_calculateEigenvectors_clicked()));
    connect(ui->calculateEigenspaces, SIGNAL(clicked(bool)), this, SLOT(on_calculateEigenspaces_clicked()));

}
void MainWindow::recreateMatrix(void){
    if (matrixWidget) {
        delete matrixWidget;    //If matrix widget is created then delete and recreate it
    }
    int n = ui->spinBox->value();

    matrixWidget = new QTableWidget(n, n, this);
    matrixWidget->setEditTriggers(QAbstractItemView::AllEditTriggers);
    if(n < 8){
        matrixWidget->setFixedSize(50 * n + 40, 50 * n + 40); //make a dynamic widget size
    }
    else{
        matrixWidget->setFixedSize(390, 390);
    }

    for (int i = 0; i < n; i++) {
            matrixWidget->setColumnWidth(i, 50); // column width
            matrixWidget->setRowHeight(i, 50);   // row height
    }
    matrixWidget->setParent(this);
    matrixWidget->move(20, 150);
    matrixWidget->show();
}
void MainWindow::on_calculateDeterminant_clicked()
{

}
void MainWindow::on_createRandomMatrix_clicked()
{
    recreateMatrix();
    int n = ui->spinBox->value();
    int rows = n;
    int cols = n;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int randomValue = QRandomGenerator::global()->bounded(-9999, 9999);
            matrixWidget->setItem(i, j, new QTableWidgetItem(QString::number(randomValue)));
        }
    }
}
void MainWindow::on_createUpperTriangularMatrix_clicked()
{
    recreateMatrix();
    int n = ui->spinBox->value();
    int rows = n;
    int cols = n;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if(i <= j){
                matrixWidget->setItem(i, j, new QTableWidgetItem("1"));
            }
            else{
                matrixWidget->setItem(i, j, new QTableWidgetItem("0"));
            }
        }
    }
}
void MainWindow::on_createLowerTriangularMatrix_clicked()
{
    recreateMatrix();
    int n = ui->spinBox->value();
    int rows = n;
    int cols = n;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if(i >= j){
                matrixWidget->setItem(i, j, new QTableWidgetItem("1"));
            }
            else{
                matrixWidget->setItem(i, j, new QTableWidgetItem("0"));
            }
        }
    }
}
void MainWindow::on_createUnitMatrix_clicked()
{
    recreateMatrix();
    int n = ui->spinBox->value();
    int rows = n;
    int cols = n;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if(i == j){
                matrixWidget->setItem(i, j, new QTableWidgetItem("1"));
            }
            else{
                matrixWidget->setItem(i, j, new QTableWidgetItem("0"));
            }
        }
    }
}
void MainWindow::on_createZeroMatrix_clicked()
{
    recreateMatrix();
    int n = ui->spinBox->value();
    int rows = n;
    int cols = n;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrixWidget->setItem(i, j, new QTableWidgetItem("0"));
        }
    }
}
void MainWindow::on_createMatrixButton_clicked()
{
    int n = ui->spinBox->value(); // dimension of the matrix

    //delete the past matrix and recreate
    if (matrixWidget) {
        delete matrixWidget;
        matrixWidget = nullptr;
    }

    matrixWidget = new QTableWidget(n, n, this);
    matrixWidget->setEditTriggers(QAbstractItemView::AllEditTriggers);
    if(n < 8){
        matrixWidget->setFixedSize(50 * n + 40, 50 * n + 40); //make a dynamic widget size
    }
    else{
        matrixWidget->setFixedSize(390, 390); //constant size for matrices bigger than 7x7
    }

    for (int i = 0; i < n; i++) {
            matrixWidget->setColumnWidth(i, 50);
            matrixWidget->setRowHeight(i, 50);
    }
    matrixWidget->setParent(this);
    matrixWidget->move(20, 150); //position of the matrix
    matrixWidget->show();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrixWidget->setItem(i, j, new QTableWidgetItem(" ")); // create empty cells
        }
    }
}
MainWindow::~MainWindow()
{
    delete ui;
}
