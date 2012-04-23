#include "refactoringWindow.h"
#include "ui_refactoringWindow.h"

using namespace qReal;

RefactoringWindow::RefactoringWindow(QWidget *parent) :
	QWidget(parent),
	mUi(new Ui::refactoringForm)
{
	mUi->setupUi(this);
}

RefactoringWindow::~RefactoringWindow()
{
	delete mUi;
}


