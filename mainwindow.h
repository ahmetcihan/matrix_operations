#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QDebug>
#include <QRandomGenerator>
#include <QVector>
#include <QMessageBox>
#include <QtMath>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QTableWidget *matrixWidget = nullptr;
    QVector<double> m_eigenvalues;

    QVector<double> computeEigenvalues();
    QVector<QVector<double>> computeEigenvectors(const QVector<double>& eigenvalues);
    QVector<QVector<QVector<double>>> computeEigenspaces(const QVector<double>& eigenvalues);

    QVector<double> solveLinearSystem(QVector<QVector<double>> A, QVector<double> b);
    QVector<QVector<double>> multiplyMatrices(const QVector<QVector<double>>& A, const QVector<QVector<double>>& B);

    void qrDecomposition(const QVector<QVector<double>>& A, QVector<QVector<double>>& Q, QVector<QVector<double>>& R);
    void normalizeVector(QVector<double>& vec);
    void normalizeSecondComponent(QVector<double>& vec);
    double vectorDistance(const QVector<double>& v1, const QVector<double>& v2);

private slots:
    void on_createMatrixButton_clicked();
    void on_createZeroMatrix_clicked();
    void on_createUnitMatrix_clicked();
    void on_createUpperTriangularMatrix_clicked();
    void on_createLowerTriangularMatrix_clicked();
    void on_createRandomMatrix_clicked();
    void on_calculateDeterminant_clicked();

    void recreateMatrix(void);
    void calculateDeterminant(void);

    void on_calculateEigenvalues_clicked();
    void on_calculateEigenvectors_clicked();
    void on_calculateEigenspaces_clicked();
};
#endif // MAINWINDOW_H
