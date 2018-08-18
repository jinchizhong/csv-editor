#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QDebug"
#include "csv.h"
#include <QFileDialog>
#include <QSettings>
#include <QClipboard>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::_getOpenFile()
{
    QSettings s;
    QString lastOpenDir = s.value("lastOpenDir").toString();

    QString fname = QFileDialog::getOpenFileName(this, "Choose csv...", lastOpenDir, "CSV Files (*.csv);;All Files(*)");
    if (!fname.isEmpty()) {
        lastOpenDir = QFileInfo(fname).absolutePath();
        s.setValue("lastOpenDir", lastOpenDir);
    }

    m_filename = fname;
    return fname;
}

void MainWindow::on_actionOpen_triggered()
{
    QString fname = _getOpenFile();

    QList<QStringList> cont = CSV::parseFromFile(fname, "UTF-8");

    ui->tableWidget->clear();

    QStringList header = cont[0];
    cont.pop_front();
    ui->tableWidget->setColumnCount(header.length());
    for (int i = 0; i < header.length(); ++i) {
        ui->tableWidget->setHorizontalHeaderItem(i, new QTableWidgetItem(header[i]));
    }

    ui->tableWidget->setRowCount(cont.length());
    for (int i = 0; i < cont.length(); ++i) {
        QStringList line = cont[i];
        for (int j = 0; j < line.length(); ++j) {
            ui->tableWidget->setItem(i, j, new QTableWidgetItem(line[j]));
        }
    }

    ui->tableWidget->resizeColumnsToContents();
}

void MainWindow::on_actionSave_triggered()
{
    QList<QStringList> data;

    QStringList header;
    for (int i = 0; i < ui->tableWidget->columnCount(); ++i) {
        header.append(ui->tableWidget->horizontalHeaderItem(i)->text());
    }
    data.append(header);

    for (int i = 0; i < ui->tableWidget->rowCount(); ++i) {
        QStringList row;
        for (int j = 0; j < ui->tableWidget->columnCount(); ++j) {
            row.append(ui->tableWidget->item(i, j)->text());
        }
        data.append(row);
    }

    CSV::write(data, m_filename, "UTF-8");
}

void MainWindow::on_actionExit_triggered()
{
    qApp->exit();
}

void MainWindow::on_actionCopy_triggered()
{
    QTableWidget * tw = ui->tableWidget;

    QClipboard * clip = QGuiApplication::clipboard();
    clip->setText(tw->currentItem()->text());
}

void MainWindow::on_actionPaste_triggered()
{
    QTableWidget * tw = ui->tableWidget;

    QClipboard * clip = QGuiApplication::clipboard();
    QString text = clip->text();

    QList<QTableWidgetItem*> si = tw->selectedItems();
    for (QTableWidgetItem* item : si) {
        item->setText(text);
    }
}

void MainWindow::on_actionClear_triggered()
{
    QTableWidget * tw = ui->tableWidget;

    QList<QTableWidgetItem*> si = tw->selectedItems();
    for (QTableWidgetItem* item : si) {
        item->setText("");
    }
}

void MainWindow::on_actionPasteAppend_triggered() {
    QTableWidget * tw = ui->tableWidget;

    QClipboard * clip = QGuiApplication::clipboard();
    QString text = clip->text();

    QList<QTableWidgetItem*> si = tw->selectedItems();
    for (QTableWidgetItem* item : si) {
        item->setText(item->text() + text);
    }
}

void MainWindow::on_actionPastePrepend_triggered()
{
    QTableWidget * tw = ui->tableWidget;

    QClipboard * clip = QGuiApplication::clipboard();
    QString text = clip->text();

    QList<QTableWidgetItem*> si = tw->selectedItems();
    for (QTableWidgetItem* item : si) {
        item->setText(text + item->text());
    }
}
