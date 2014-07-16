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
	//, mCurrentPenWidth(0)
	//, mCurrentPenColor(Qt::black)
{
	mEngine.display()->setPainter(this);
}

//void Display::setPainterColor(QColor const &color)
//{
//	mCurrentPenColor = color;
//}

//void Display::setPainterWidth(int penWidth)
//{
//	mCurrentPenWidth = penWidth;
//}

void Display::drawLine(int x1, int y1, int x2, int y2)
{
	mLines << LineCoordinates(x1, y1, x2, y2, Qt::black/*mCurrentPenColor*/, 0/*mCurrentPenWidth*/);
	mEngine.display()->repaintDisplay();
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
	mLines.clear();
}

void Display::paint(QPainter *painter)
{
	/// @todo ZOMG.
	qreal const pixWidth = static_cast<qreal>(mEngine.display()->displayWidth()) / nxtDisplayWidth;
	qreal const pixHeight = static_cast<qreal>(mEngine.display()->displayHeight()) / nxtDisplayHeight;

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

	for (int i = 0; i < mLines.length(); ++i)
	{
		//painter->setPen(QPen(mLines.at(i).color, mLines.at(i).penWidth, Qt::SolidLine, Qt::SquareCap, Qt::BevelJoin));
		painter->drawLine(mLines.at(i).coord1.x() * pixWidth, mLines.at(i).coord1.y() * pixHeight
				, mLines.at(i).coord2.x() * pixWidth, mLines.at(i).coord2.y() * pixHeight);
	}

	painter->restore();
}

void Display::clear()
{
	clearScreen();
}
