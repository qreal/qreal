#include "nxtdisplay.h"
#include "ui_nxtdisplay.h"

NxtDisplay::NxtDisplay(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NxtDisplay)
{
    ui->setupUi(this);
}

NxtDisplay::~NxtDisplay(){
    delete ui;
}

void NxtDisplay::on_leftButton_clicked(){
    emit left();
}

void NxtDisplay::on_centralButton_clicked(){
    emit central();
}

void NxtDisplay::on_rightButton_clicked(){
    emit right();
}

void NxtDisplay::on_bottomButton_clicked(){
    emit bottom();
}
