#include "nxtDisplay.h"
#include "ui_nxtdisplay.h"

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

void NxtDisplay::onLeftButtonClicked()
{
	emit left();
}

void NxtDisplay::onCentralButtonClicked()
{
	emit central();
}

void NxtDisplay::onRightButtonClicked()
{
	emit right();
}

void NxtDisplay::onBottomButtonClicked()
{
	emit bottom();
}
