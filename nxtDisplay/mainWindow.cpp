#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, mUI(new Ui::MainWindow)
{
	mUI->setupUi(this);
}

MainWindow::~MainWindow()
{
	delete mUI;
}
