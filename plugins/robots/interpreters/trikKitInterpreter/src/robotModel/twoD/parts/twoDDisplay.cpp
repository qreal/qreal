#include "twoDDisplay.h"

using namespace trikKitInterpreter::robotModel::twoD::parts;
using namespace interpreterBase::robotModel;

Display::Display(DeviceInfo const &info
		, PortInfo const &port
		, twoDModel::engine::TwoDModelEngineInterface &engine)
	: robotModel::parts::TrikDisplay(info, port)
	, mEngine(engine)
	, mBackground(Qt::transparent)
{
	mEngine.display()->setPainter(this);
}

void Display::drawSmile(bool sad)
{
	mCurrentImage = QImage(sad ? ":/icons/sadSmile.png" : ":/icons/smile.png");
	mEngine.display()->repaintDisplay();
}

void Display::setBackground(QColor const &color)
{
	mBackground = color;
	mEngine.display()->repaintDisplay();
}

void Display::clearScreen()
{
	mCurrentImage = QImage();
	mBackground = Qt::transparent;
	if (mEngine.display()) {
		mEngine.display()->repaintDisplay();
	}
}

void Display::paint(QPainter *painter)
{
	QRect const displayRect(0, 0, mEngine.display()->displayWidth(), mEngine.display()->displayHeight());
	painter->save();
	painter->setPen(mBackground);
	painter->setBrush(mBackground);
	painter->drawRect(displayRect);
	painter->drawImage(displayRect, mCurrentImage);
	painter->restore();
}

void Display::clear()
{
	clearScreen();
}
