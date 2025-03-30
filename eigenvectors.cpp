#include "mainwindow.h"
#include "ui_mainwindow.h"


void MainWindow::on_calculateEigenvectors_clicked() {
    if (!matrixWidget || m_eigenvalues.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please calculate eigenvalues first!");
        return;
    }

    QVector<QVector<double>> eigenvectors = computeEigenvectors(m_eigenvalues);

    QStringList results;
    for (int i = 0; i < eigenvectors.size(); ++i) {
        QStringList components;
        for (int j = 0; j < eigenvectors[i].size(); ++j) {
            components << QString::number(eigenvectors[i][j], 'f', 4);
        }
        results << QString("EVec for λ%1: [ %2 ]").arg(i+1).arg(components.join(" "));
    }
    ui->labelEigenvectors->setText(results.join("\n"));
}

QVector<QVector<double>> MainWindow::computeEigenvectors(const QVector<double>& eigenvalues) {
    int n = ui->spinBox->value();
    QVector<QVector<double>> matrix(n, QVector<double>(n));

    // Get matrix from UI
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            QTableWidgetItem *item = matrixWidget->item(i, j);
            bool ok;
            double value = item ? item->text().toDouble(&ok) : 0.0;
            if (!item || !ok) {
                QMessageBox::warning(this, "Error", "Invalid matrix entries!");
                return QVector<QVector<double>>();
            }
            matrix[i][j] = value;
        }
    }

    QVector<QVector<double>> eigenvectors;

    for (double lambda : eigenvalues) {
        QVector<QVector<double>> A_minus_lambdaI = matrix;
        for (int i = 0; i < n; ++i) {
            A_minus_lambdaI[i][i] -= lambda;
        }

        QVector<double> eigenvector(n, 1.0); // Initial guess

        // Try multiple initial guesses for robustness
         for (int trial = 0; trial < 3; ++trial) {
             QVector<double> b(n);
             for (int i = 0; i < n; ++i) {
                 b[i] = QRandomGenerator::global()->generateDouble() * 4.0 - 2.0; // [-2.0, 2.0)
             }

             for (int iter = 0; iter < 50; ++iter) {
                 QVector<double> new_b = solveLinearSystem(A_minus_lambdaI, b);
                 if (new_b == QVector<double>(n, 0.0)) break; // Singular case

                 normalizeVector(new_b);
                 normalizeSecondComponent(new_b); // 2. Wolfram-style normalize (ikinci bileşen = 1)

                 if (vectorDistance(b, new_b) < 1e-8) break; // Stricter convergence check
                 b = new_b;
             }

             if (!b.isEmpty() && b != QVector<double>(n, 0.0)) {
                 eigenvector = b;
                 break;
             }
         }

        // Fallback for singular cases (2x2 matrices)
        if (n == 2 && eigenvector == QVector<double>(n, 0.0)) {
            double a = A_minus_lambdaI[0][0], b = A_minus_lambdaI[0][1];
            double c = A_minus_lambdaI[1][0], d = A_minus_lambdaI[1][1];

            if (qFuzzyIsNull(a) && qFuzzyIsNull(b)) {
                eigenvector = { -d, c };
            } else {
                eigenvector = { -b, a };
            }
            normalizeVector(eigenvector);
        }

        eigenvectors.append(eigenvector);
    }

    return eigenvectors;
}

QVector<double> MainWindow::solveLinearSystem(QVector<QVector<double>> A, QVector<double> b) {
    int n = A.size();

     // Check for singular matrix (2x2 case)
     if (n == 2) {
         double det = A[0][0]*A[1][1] - A[0][1]*A[1][0];
         if (qFuzzyIsNull(det)) {
             if (!qFuzzyIsNull(A[0][0]) || !qFuzzyIsNull(A[0][1])) {
                 return QVector<double>{-A[0][1], A[0][0]}; // [-b, a]
             } else {
                 return QVector<double>{-A[1][1], A[1][0]}; // [-d, c]
             }
         }
     }

     // Gaussian Elimination with Partial Pivoting
     for (int col = 0; col < n; ++col) {
         // Partial pivoting
         int pivot = col;
         for (int i = col + 1; i < n; ++i) {
             if (qAbs(A[i][col]) > qAbs(A[pivot][col])) {
                 pivot = i;
             }
         }

         if (qFuzzyIsNull(A[pivot][col])) {
             continue; // Skip zero column
         }

         std::swap(A[col], A[pivot]);
         std::swap(b[col], b[pivot]);

         // Elimination
         for (int i = col + 1; i < n; ++i) {
             double factor = A[i][col] / A[col][col];
             for (int j = col; j < n; ++j) {
                 A[i][j] -= factor * A[col][j];
             }
             b[i] -= factor * b[col];
         }
     }

     // Back substitution
     QVector<double> x(n, 0.0);
     for (int i = n - 1; i >= 0; --i) {
         if (qFuzzyIsNull(A[i][i])) {
             x[i] = 1.0; // Free variable
             continue;
         }

         x[i] = b[i];
         for (int j = i + 1; j < n; ++j) {
             x[i] -= A[i][j] * x[j];
         }
         x[i] /= A[i][i];
     }

     return x;}

void MainWindow::normalizeVector(QVector<double>& vec) {
    double norm = 0.0;
    for (double val : vec) {
        norm += val * val;
    }
    norm = qSqrt(norm);

    if (norm > 1e-10) {
        for (double &val : vec) {
            val /= norm;
        }
    } else {
        vec.fill(1.0);
        normalizeVector(vec);
    }
}
void MainWindow::normalizeSecondComponent(QVector<double>& vec) {
    if (vec.size() >= 2 && !qFuzzyIsNull(vec[1])) {
        double scale = 1.0 / vec[1];
        for (int i = 0; i < vec.size(); ++i) {
            vec[i] *= scale;
        }
    } else {
        // Fallback: Varsayılan normalizasyon
        normalizeVector(vec);
    }
}
double MainWindow::vectorDistance(const QVector<double>& v1, const QVector<double>& v2) {
    double dist = 0.0;
    for (int i = 0; i < v1.size(); ++i) {
        dist += (v1[i] - v2[i]) * (v1[i] - v2[i]);
    }
    return qSqrt(dist);
}
