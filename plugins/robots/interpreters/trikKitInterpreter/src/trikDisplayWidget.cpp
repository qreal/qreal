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

bool TrikDisplayWidget::buttonIsDown(QString const &buttonPort) const
{
	if (buttonPort == "LeftButtonPort") {
		return mUi->buttonLeft->isDown();
	} else if (buttonPort == "RightButtonPort") {
		return mUi->buttonRight->isDown();
	} else if (buttonPort == "UpButtonPort") {
		return mUi->buttonUp->isDown();
	} else if (buttonPort == "DownButtonPort") {
		return mUi->buttonDown->isDown();
	} else if (buttonPort == "EnterButtonPort") {
		return mUi->buttonDownLeft->isDown();
	} else if (buttonPort == "EscapeButtonPort") {
		return mUi->buttonAttach->isDown();
	} else if (buttonPort == "PowerButtonPort") {
		return mUi->buttonOn->isDown();
	}

	throw qReal::Exception("Incorrect button id in TrikDisplayWidget::buttonIsDown");
}

/// set led color on display
void TrikDisplayWidget::setLedColor(QColor const &color)
{
	/// make palette
	QPalette Pal(palette());
	/// set color background
	Pal.setColor(QPalette::Background, color);
	mUi->led->setAutoFillBackground(true);
	mUi->led->setPalette(Pal);
	mUi->led->show();
}

void TrikDisplayWidget::paintEvent(QPaintEvent *event)
{
	QWidget::paintEvent(event);
	QPainter painter(this);
	painter.drawImage(QRect(0, 0, width(), height()), mBackground);
}
