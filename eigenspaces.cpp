#include "mainwindow.h"
#include "ui_mainwindow.h"

QVector<QVector<QVector<double>>> MainWindow::computeEigenspaces(const QVector<double>& eigenvalues) {
    QVector<QVector<QVector<double>>> eigenspaces;
    QVector<QVector<double>> eigenvectors = computeEigenvectors(eigenvalues);

    for (int i = 0; i < eigenvalues.size(); ++i) {
        QVector<QVector<double>> basis;
        basis.append(eigenvectors[i]); // Her eigenvalue için bir eigenvector
        eigenspaces.append(basis);
    }

    return eigenspaces;
}
void MainWindow::on_calculateEigenspaces_clicked() {
    if (!matrixWidget || m_eigenvalues.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please calculate eigenvalues first!");
        return;
    }

    QVector<QVector<QVector<double>>> eigenspaces = computeEigenspaces(m_eigenvalues);
    QStringList results;

    for (int i = 0; i < eigenspaces.size(); ++i) {
        QStringList vectors;
        for (const QVector<double>& vec : eigenspaces[i]) {
            QStringList components;
            for (double val : vec) {
                components << QString::number(val, 'f', 4);
            }
            vectors << QString("[ %1 ]").arg(components.join(" "));
        }
        results << QString("ES for λ%1: Span{ %2 }").arg(i+1).arg(vectors.join(", "));
    }
    ui->textEditEigenspaces->setPlainText(results.join("\n"));
    ui->textEditEigenspaces->moveCursor(QTextCursor::Start);
}
