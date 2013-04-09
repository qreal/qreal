#include "nxtDisplay.h"
#include "ui_nxtDisplay.h"

using namespace qReal::interpreters::robots::details;

NxtDisplay::NxtDisplay(QWidget *parent)
	: QWidget(parent)
	, mUi(new Ui::NxtDisplay)
{
	mUi->setupUi(this);
}

NxtDisplay::~NxtDisplay()
{
	delete mUi;
}

bool NxtDisplay::leftButtonIsDown(){
	return mUi->leftButton->isDown();
}

bool NxtDisplay::rightButtonIsDown(){
	return mUi->rightButton->isDown();
}

bool NxtDisplay::centralButtonIsDown(){
	return mUi->centralButton->isDown();
}

bool NxtDisplay::bottomButtonIsDown(){
	return mUi->bottomButton->isDown();
}
