#include "trikDisplayWidget.h"
#include "ui_trikDisplayWidget.h"

#include <qrkernel/exception/exception.h>
#include <qrkernel/logging.h>

using namespace trik;

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
	TwoDModelDisplayWidget::setPainter(painter);
	mUi->display->appendPainter(painter);
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

bool TrikDisplayWidget::buttonIsDown(const QString &buttonPort) const
{
	if (buttonPort == "Left") {
		return mUi->buttonLeft->isDown();
	} else if (buttonPort == "Right") {
		return mUi->buttonRight->isDown();
	} else if (buttonPort == "Up") {
		return mUi->buttonUp->isDown();
	} else if (buttonPort == "Down") {
		return mUi->buttonDown->isDown();
	} else if (buttonPort == "Enter") {
		return mUi->buttonEnter->isDown();
	} else if (buttonPort == "Esc") {
		return mUi->buttonEscape->isDown();
	} else if (buttonPort == "Power") {
		return mUi->buttonPower->isDown();
	}

	QLOG_WARN() << "Button on port" << buttonPort << "is not supported by 2d model";

	return false;
}

void TrikDisplayWidget::setLedColor(const QColor &color)
{
	QPalette backgroundPalette(palette());
	backgroundPalette.setColor(QPalette::Background, color);
	mUi->led->setPalette(backgroundPalette);
	mUi->led->show();
}

void TrikDisplayWidget::paintEvent(QPaintEvent *event)
{
	QWidget::paintEvent(event);
	QPainter painter(this);
	painter.drawImage(QRect(0, 0, width(), height()), mBackground);
}
