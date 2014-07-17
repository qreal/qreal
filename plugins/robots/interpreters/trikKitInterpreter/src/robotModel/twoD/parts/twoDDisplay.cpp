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
	, mCurrentPenWidth(0)
	, mCurrentPenColor(Qt::black)
{
	mEngine.display()->setPainter(this);
}

void Display::setPainterColor(QColor const &color)
{
	mCurrentPenColor = color;
}

void Display::setPainterWidth(int penWidth)
{
	mCurrentPenWidth = penWidth;
}

void Display::drawPixel(int x, int y)
{
	mPixels << PixelCoordinates(x, y, mCurrentPenColor, mCurrentPenWidth);
	mEngine.display()->repaintDisplay();
}

void Display::drawLine(int x1, int y1, int x2, int y2)
{
	mLines << LineCoordinates(x1, y1, x2, y2, mCurrentPenColor, mCurrentPenWidth);
	mEngine.display()->repaintDisplay();
}

void Display::drawRect(int x, int y, int width, int height)
{
	mRects << RectCoordinates(x, y, width, height, mCurrentPenColor, mCurrentPenWidth);
	mEngine.display()->repaintDisplay();
}

void Display::drawEllipse(int x, int y, int width, int height)
{
	mEllipses << EllipseCoordinates(x, y, width, height, mCurrentPenColor, mCurrentPenWidth);
	mEngine.display()->repaintDisplay();
}

void Display::drawArc(int x, int y, int width, int height, int startAngle, int spanAngle)
{
	mArcs << ArcCoordinates(x, y, width, height, startAngle, spanAngle, mCurrentPenColor, mCurrentPenWidth);
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
	mPixels.clear();
	mLines.clear();
	mRects.clear();
	mEllipses.clear();
	mArcs.clear();
	if (mEngine.display()) {
		mEngine.display()->repaintDisplay();
	}
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

	for (int i = 0; i < mPixels.length(); ++i)
	{
		painter->setPen(QPen(mPixels.at(i).color, mPixels.at(i).penWidth, Qt::SolidLine, Qt::SquareCap, Qt::BevelJoin));
		painter->drawPoint(mPixels.at(i).coord.x() * pixWidth, mPixels.at(i).coord.y() * pixHeight);
	}

	for (int i = 0; i < mLines.length(); ++i)
	{
		painter->setPen(QPen(mLines.at(i).color, mLines.at(i).penWidth, Qt::SolidLine, Qt::SquareCap, Qt::BevelJoin));
		painter->drawLine(mLines.at(i).line.x1() * pixWidth, mLines.at(i).line.y1() * pixHeight
				, mLines.at(i).line.x2() * pixWidth, mLines.at(i).line.y2() * pixHeight);
	}

	for (int i = 0; i < mRects.length(); ++i)
	{
		painter->setPen(QPen(mRects.at(i).color, mRects.at(i).penWidth, Qt::SolidLine, Qt::SquareCap, Qt::BevelJoin));
		painter->drawRect(mRects.at(i).rect.x() * pixWidth, mRects.at(i).rect.y() * pixHeight
				, mRects.at(i).rect.width() * pixWidth, mRects.at(i).rect.height() * pixHeight);
	}

	for (int i = 0; i < mEllipses.length(); ++i)
	{
		painter->setPen(QPen(mEllipses.at(i).color, mEllipses.at(i).penWidth, Qt::SolidLine, Qt::SquareCap, Qt::BevelJoin));
		painter->drawEllipse(mEllipses.at(i).ellipse.x() * pixWidth, mEllipses.at(i).ellipse.y() * pixHeight
				, mEllipses.at(i).ellipse.width() * pixWidth, mEllipses.at(i).ellipse.height() * pixHeight);
	}

	for (int i = 0; i < mArcs.length(); ++i)
	{
		painter->setPen(QPen(mArcs.at(i).color, mArcs.at(i).penWidth, Qt::SolidLine, Qt::SquareCap, Qt::BevelJoin));
		painter->drawArc(mArcs.at(i).arc.x() * pixWidth, mArcs.at(i).arc.y() * pixHeight
				, mArcs.at(i).arc.width() * pixWidth, mArcs.at(i).arc.height() * pixHeight
				, mArcs.at(i).startAngle, mArcs.at(i).spanAngle);
	}

	painter->restore();
}

void Display::clear()
{
	clearScreen();
}
