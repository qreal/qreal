#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::on_LeftButton_clicked(){
    emit leftButtonClicked();
}

void MainWindow::on_CentralButton_clicked(){
    emit centralButtonClicked();
}

void MainWindow::on_RightButton_clicked(){
    emit rightButtonClicked();
}

void MainWindow::on_LowerButton_clicked(){
    emit lowerButtonClicked();
}
