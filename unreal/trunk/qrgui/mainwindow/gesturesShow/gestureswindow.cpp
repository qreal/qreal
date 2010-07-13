#include "gestureswindow.h"
#include "ui_gestureswindow.h"

GesturesWindow::GesturesWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GesturesWindow)
{
    ui->setupUi(this);
}

GesturesWindow::~GesturesWindow()
{
    delete ui;
}
