#include "nxtDisplay.h"
#include "ui_nxtDisplay.h"

using namespace qReal::interpreters::robots::details;

NxtDisplay::NxtDisplay(QWidget *parent)
	: QWidget(parent)
	, mUI(new Ui::NxtDisplay)
{
	mUI->setupUi(this);
}

NxtDisplay::~NxtDisplay()
{
	delete mUI;
}

bool NxtDisplay::leftButtonIsDown(){
	return mUI->leftButton->isDown();
}

bool NxtDisplay::rightButtonIsDown(){
	return mUI->rightButton->isDown();
}

bool NxtDisplay::centralButtonIsDown(){
	return mUI->centralButton->isDown();
}

bool NxtDisplay::bottomButtonIsDown(){
	return mUI->bottomButton->isDown();
}
