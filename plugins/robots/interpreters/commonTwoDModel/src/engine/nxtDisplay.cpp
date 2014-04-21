#include "nxtDisplay.h"

#include "ui_nxtDisplay.h"

#include <qrutils/graphicsUtils/painterInterface.h>

using namespace twoDModel;
using namespace graphicsUtils;

NxtDisplay::NxtDisplay(QWidget *parent)
	: QWidget(parent)
	, mUi(new Ui::NxtDisplay)
	, mBackground(":/icons/background.png", "PNG")
{
	mUi->setupUi(this);
}

NxtDisplay::~NxtDisplay()
{
	delete mUi;
}

bool NxtDisplay::leftButtonIsDown() const
{
	return mUi->leftButton->isDown();
}

bool NxtDisplay::rightButtonIsDown() const
{
	return mUi->rightButton->isDown();
}

bool NxtDisplay::centralButtonIsDown() const
{
	return mUi->centralButton->isDown();
}

bool NxtDisplay::bottomButtonIsDown() const
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

int NxtDisplay::displayWidth() const
{
	return mUi->display->width();
}

int NxtDisplay::displayHeight() const
{
	return mUi->display->height();
}

void NxtDisplay::paintEvent(QPaintEvent *event)
{
	QWidget::paintEvent(event);
	QPainter painter(this);
	painter.drawImage(QRect(0, 0, width(), height()), mBackground);
}
