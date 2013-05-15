#include "nxtDisplay.h"
#include "ui_nxtDisplay.h"

#include "../../../../qrutils/graphicsUtils/painterInterface.h"

using namespace qReal::interpreters::robots::details;
using namespace graphicsUtils;

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

bool NxtDisplay::leftButtonIsDown()
{
	return mUi->leftButton->isDown();
}

bool NxtDisplay::rightButtonIsDown()
{
	return mUi->rightButton->isDown();
}

bool NxtDisplay::centralButtonIsDown()
{
	return mUi->centralButton->isDown();
}

bool NxtDisplay::bottomButtonIsDown()
{
	return mUi->bottomButton->isDown();
}

void NxtDisplay::setPainter(graphicsUtils::PainterInterface *p)
{
	mUi->display->setPainter(p);
}

void NxtDisplay::repaintDisplay()
{
	mUi->display->repaint();
}

int NxtDisplay::displayWidth()
{
	return mUi->display->width();
}

int NxtDisplay::displayHeight()
{
	return mUi->display->height();
}
