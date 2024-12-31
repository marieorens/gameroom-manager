#include "mainwindow.h"
#include "dialog.h"
#include "ui_mainwindow.h"
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Configuration");
    this->setWindowIcon(QIcon());

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    Dialog acceuil;
    acceuil.setModal(true);
    acceuil.exec();
}


