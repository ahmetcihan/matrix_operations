#include "mainwindow.h"
#include "ui_mainwindow.h"

// Recursive determinant hesaplayan fonksiyon
double determinant(const QVector<QVector<double>> &mat) {
    int size = mat.size();
    if (size == 1) return mat[0][0];
    if (size == 2) return mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0];

    double det = 0.0;
    for (int col = 0; col < size; col++) {
        QVector<QVector<double>> subMatrix(size - 1, QVector<double>(size - 1));

        for (int i = 1; i < size; i++) {
            int subCol = 0;
            for (int j = 0; j < size; j++) {
                if (j == col) continue;
                subMatrix[i - 1][subCol] = mat[i][j];
                subCol++;
            }
        }
        det += ((col % 2 == 0) ? 1 : -1) * mat[0][col] * determinant(subMatrix);
    }
    return det;
}

void MainWindow::calculateDeterminant() {
    if (!matrixWidget) {
        qDebug() << "Please initilize the matrix first";    //just for crash protection
        return;
    }
    int n = ui->spinBox->value();
    //create a nxn matrix
    QVector<QVector<double>> matrix;
    matrix.resize(n);  // Create 'n' rows
    for (int i = 0; i < n; i++) {
        matrix[i].resize(n);  // Each row has 'n' columns
    }

    // get values from the user interface matrix and assign to "matrix" variable
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            QTableWidgetItem *item = matrixWidget->item(i, j);
            //check if the cell value is a number or not
            bool ok;
            double value = item ? item->text().toDouble(&ok) : 0.0;
            if (!item || !ok) {
                QMessageBox::warning(this, "Error", "Please enter valid numbers in all matrix cells.");
                return;
            }
            matrix[i][j] = value;
        }
    }

    double detValue = determinant(matrix);
    ui->label_determinant->setText(QString("Determinant: %1").arg(detValue, 0, 'f', 0));
}
