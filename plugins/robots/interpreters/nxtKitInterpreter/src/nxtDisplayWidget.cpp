#include "nxtDisplayWidget.h"

#include "ui_nxtDisplayWidget.h"

#include <qrutils/graphicsUtils/painterInterface.h>
#include <qrkernel/exception/exception.h>

using namespace nxtKitInterpreter;
using namespace graphicsUtils;

NxtDisplayWidget::NxtDisplayWidget(QWidget *parent)
	: twoDModel::engine::TwoDModelDisplayWidget(parent)
	, mUi(new Ui::NxtDisplayWidget)
	, mBackground(":/icons/background.png", "PNG")
{
	mUi->setupUi(this);
}

NxtDisplayWidget::~NxtDisplayWidget()
{
	delete mUi;
}

bool NxtDisplayWidget::buttonIsDown(QString const &buttonPort) const
{
	if (buttonPort == "LeftButtonPort") {
		return mUi->leftButton->isDown();
	} else if (buttonPort == "RightButtonPort") {
		return mUi->rightButton->isDown();
	} else if (buttonPort == "EnterButtonPort") {
		return mUi->centralButton->isDown();
	} else if (buttonPort == "EscapeButtonPort") {
		return mUi->bottomButton->isDown();
	}

	throw qReal::Exception("Incorrect button id in NxtDisplayWidget::buttonIsDown");
}

void NxtDisplayWidget::setPainter(graphicsUtils::PainterInterface *painter)
{
	TwoDModelDisplayWidget::setPainter(painter);
	mUi->display->appendPainter(painter);
}

void NxtDisplayWidget::repaintDisplay()
{
	mUi->display->repaint();
}

int NxtDisplayWidget::displayWidth() const
{
	return mUi->display->width();
}

int NxtDisplayWidget::displayHeight() const
{
	return mUi->display->height();
}

void NxtDisplayWidget::paintEvent(QPaintEvent *event)
{
	QWidget::paintEvent(event);
	QPainter painter(this);
	painter.drawImage(QRect(0, 0, width(), height()), mBackground);
}
