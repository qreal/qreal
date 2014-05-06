#include "trikDisplayWidget.h"
#include "ui_trikDisplayWidget.h"

#include <qrkernel/exception/exception.h>

using namespace trikKitInterpreter;

TrikDisplayWidget::TrikDisplayWidget(QWidget *parent)
	: twoDModel::engine::TwoDModelDisplayWidget(parent)
	, mUi(new Ui::TrikDisplayWidget)
	, mBackground(":/icons/controller.png", "PNG")
{
	mUi->setupUi(this);
}

TrikDisplayWidget::~TrikDisplayWidget()
{
	delete mUi;
}

void TrikDisplayWidget::setPainter(graphicsUtils::PainterInterface *painter)
{
	mUi->display->setPainter(painter);
}

void TrikDisplayWidget::repaintDisplay()
{
	mUi->display->repaint();
}

int TrikDisplayWidget::displayWidth() const
{
	return mUi->display->width();
}

int TrikDisplayWidget::displayHeight() const
{
	return mUi->display->height();
}

bool TrikDisplayWidget::buttonIsDown(QString const &buttonId) const
{
	if (buttonId == "left") {
		return mUi->buttonLeft->isDown();
	} else if (buttonId == "right") {
		return mUi->buttonRight->isDown();
	} else if (buttonId == "up") {
		return mUi->buttonUp->isDown();
	} else if (buttonId == "down") {
		return mUi->buttonDown->isDown();
	} else if (buttonId == "enter") {
		return mUi->buttonDownLeft->isDown();
	} else if (buttonId == "escape") {
		return mUi->buttonAttach->isDown();
	} else if (buttonId == "power") {
		return mUi->buttonOn->isDown();
	}

	throw qReal::Exception("Incorrect button id in TrikDisplayWidget::buttonIsDown");
}

void TrikDisplayWidget::paintEvent(QPaintEvent *event)
{
	QWidget::paintEvent(event);
	QPainter painter(this);
	painter.drawImage(QRect(0, 0, width(), height()), mBackground);
}
