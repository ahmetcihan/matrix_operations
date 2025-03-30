#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::on_calculateEigenvalues_clicked() {
    if (!matrixWidget) {
        QMessageBox::warning(this, "Error", "Please initialize the matrix first!");
        return;
    }

    QVector<double> eigenvalues = computeEigenvalues();
    if (eigenvalues.isEmpty()){
        ui->labelEigenvalues->setText("<span style='color:red;'>Eigenvalues not available</span>");
    return;
    }

    //Show results
    QString result;
    for (int i = 0; i < eigenvalues.size(); ++i) {
        result += QString("λ%1: %2\n").arg(i+1).arg(eigenvalues[i], 0, 'f', 4);
    }
    ui->labelEigenvalues->setText(result);
}

QVector<double> MainWindow::computeEigenvalues() {
    int n = ui->spinBox->value();
    QVector<QVector<double>> matrix(n, QVector<double>(n));

    // get values from the user interface matrix and assign to "matrix" variable
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            QTableWidgetItem *item = matrixWidget->item(i, j);
            //check if the cell value is a number or not
            bool ok;
            double value = item ? item->text().toDouble(&ok) : 0.0;
            if (!item || !ok) {
                QMessageBox::warning(this, "Error", "Please enter valid numbers in all matrix cells.");
                return QVector<double>();
            }

            matrix[i][j] = value;
        }
    }

    // QR Algorithm
    QVector<QVector<double>> A = matrix;
    QVector<QVector<double>> Q, R;

    for (int iter = 0; iter < 50; ++iter) {
        qrDecomposition(A, Q, R);
        A = multiplyMatrices(R, Q);
    }

    // Diagonal elements are eigenvalues
    QVector<double> eigenvalues(n);
    for (int i = 0; i < n; ++i) eigenvalues[i] = A[i][i];

    return eigenvalues;
}
void MainWindow::qrDecomposition(const QVector<QVector<double>>& A,
                               QVector<QVector<double>>& Q,
                               QVector<QVector<double>>& R) {
    int n = A.size();
    Q = QVector<QVector<double>>(n, QVector<double>(n, 0.0));
    R = A;

    for (int k = 0; k < n-1; ++k) {
        // Householder vector
        QVector<double> x(n, 0.0);
        for (int i = k; i < n; ++i) x[i] = R[i][k];

        double norm = 0.0;
        for (int i = k; i < n; ++i) norm += x[i] * x[i];
        norm = sqrt(norm);

        x[k] += (x[k] < 0 ? -norm : norm);
        double beta = 2.0 / std::inner_product(x.begin(), x.end(), x.begin(), 0.0);

        //update R
        for (int j = k; j < n; ++j) {
            double dot = 0.0;
            for (int i = k; i < n; ++i) dot += x[i] * R[i][j];
            for (int i = k; i < n; ++i) R[i][j] -= beta * x[i] * dot;
        }

        //create Q
        if (k == 0) {
            for (int i = 0; i < n; ++i) Q[i][i] = 1.0;
        }
        for (int j = 0; j < n; ++j) {
            double dot = 0.0;
            for (int i = k; i < n; ++i) dot += Q[j][i] * x[i];
            for (int i = k; i < n; ++i) Q[j][i] -= beta * x[i] * dot;
        }
    }
}
QVector<QVector<double>> MainWindow::multiplyMatrices(
    const QVector<QVector<double>>& A,
    const QVector<QVector<double>>& B)
{
    // 1. Validate dimensions first
    if (A.isEmpty() || B.isEmpty() || A[0].size() != B.size()) {
        qWarning() << "Invalid dimensions: A columns (" << A[0].size()
                  << ") must match B rows (" << B.size() << ")";
        return {};
    }

    // 2. Initialize result matrix
    const int n = A.size();     // Rows of A
    const int m = B[0].size();  // Columns of B
    const int p = B.size();     // Columns of A / Rows of B (must be equal)

    QVector<QVector<double>> C(n, QVector<double>(m, 0.0));

    // 3. Perform multiplication (no internal checks needed)
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            for (int k = 0; k < p; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    return C;
}
