#include "nxtDisplay.h"

#include "ui_nxtDisplay.h"

#include <qrutils/graphicsUtils/painterInterface.h>
#include <qrkernel/exception/exception.h>

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

bool NxtDisplay::buttonIsDown(QString const &buttonId) const
{
	if (buttonId == "left") {
		return mUi->leftButton->isDown();
	} else if (buttonId == "right") {
		return mUi->rightButton->isDown();
	} else if (buttonId == "central") {
		return mUi->centralButton->isDown();
	} else if (buttonId == "bottom") {
		return mUi->bottomButton->isDown();
	}

	throw qReal::Exception("Incorrect button id in NxtDisplay::buttonIsDown");
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
