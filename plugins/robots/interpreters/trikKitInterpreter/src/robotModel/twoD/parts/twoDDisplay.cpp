#include "twoDDisplay.h"

using namespace trikKitInterpreter::robotModel::twoD::parts;
using namespace interpreterBase::robotModel;

/// @todo: This constant adjusts screen coordinates shift. It must be 0.
int const yDisplayShift = 8;

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

void Display::printText(int x, int y, QString const &text)
{
	mLabels[qMakePair(x, y + yDisplayShift)] = text;
	mEngine.display()->repaintDisplay();
}

void Display::clearScreen()
{
	mCurrentImage = QImage();
	mBackground = Qt::transparent;
	mLabels.clear();
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

	painter->save();
	painter->setPen(Qt::black);

	for (QPair<int, int> const &point : mLabels.keys()) {
		/// @todo: Honest labels must be here, without text overlapping.
		painter->drawText(QPoint(point.first, point.second), mLabels[point]);
	}

	painter->restore();
}

void Display::clear()
{
	clearScreen();
}
