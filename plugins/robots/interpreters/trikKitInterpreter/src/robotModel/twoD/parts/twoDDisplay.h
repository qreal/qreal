#pragma once

#include "robotModel/parts/trikDisplay.h"

#include <commonTwoDModel/engine/twoDModelEngineInterface.h>
#include <commonTwoDModel/engine/twoDModelDisplayInterface.h>

namespace trikKitInterpreter {
namespace robotModel {
namespace twoD {
namespace parts {

/// @todo Move this somewhere.
int const textPixelHeight = 8;
int const textPixelWidth = 16;
int const nxtDisplayHeight = 64;
int const nxtDisplayWidth = 100;

class Display : public robotModel::parts::TrikDisplay, public graphicsUtils::PainterInterface
{
	Q_OBJECT

public:
	Display(interpreterBase::robotModel::DeviceInfo const &info
			, interpreterBase::robotModel::PortInfo const &port
			, twoDModel::engine::TwoDModelEngineInterface &engine);

	void drawSmile(bool sad) override;
	void setBackground(QColor const &color) override;
	void printText(int x, int y, QString const &text) override;
	void clearScreen() override;
	//void setPainterColor(QColor const &color) override;
	//void setPainterWidth(int penWidth) override;
	void drawPixel(int x, int y) override;
	void drawLine(int x1, int y1, int x2, int y2) override;
	void drawRect(int x, int y, int width, int height)  override;
	void drawEllipse(int x, int y, int width, int height) override;
	void drawArc(int x, int y, int width, int height, int startAngle, int spanAngle) override;

	void paint(QPainter *painter) override;
	void clear() override;

private:
	twoDModel::engine::TwoDModelEngineInterface &mEngine;
	QColor mBackground;
	QImage mCurrentImage;
	/// @todo: QPoint can`t be used in map without operators declaration.
	QHash<QPair<int, int>, QString> mLabels;

	struct PixelCoordinates
	{
		PixelCoordinates(int x, int y, QColor color, int penWidth)
				: coord(QPoint(x, y)), color(color), penWidth(penWidth)
		{
		}

		QPoint coord;
		QColor color;
		int penWidth;
	};

	struct LineCoordinates
	{
		LineCoordinates(int x1, int y1, int x2, int y2, QColor color, int penWidth)
			: line(x1, y1, x2, y2), color(color), penWidth(penWidth)
		{
		}

		QLine line;
		QColor color;
		int penWidth;
	};

	struct RectCoordinates
	{
		RectCoordinates(int x, int y, int width, int height, QColor color, int penWidth)
			: rect(QRect(x, y, width, height)), color(color), penWidth(penWidth)
		{
		}

		QRect rect;
		QColor color;
		int penWidth;
	};

	struct EllipseCoordinates
	{
		EllipseCoordinates(int x, int y, int width, int height, QColor color, int penWidth)
			: ellipse(QRect(x, y, width, height)), color(color), penWidth(penWidth)
		{
		}

		QRect ellipse;
		QColor color;
		int penWidth;
	};

	struct ArcCoordinates
	{
		ArcCoordinates(int x, int y, int width, int height, int startAngle, int spanAngle, QColor color, int penWidth)
			: arc(QRect(x, y, width, height))
			, startAngle(startAngle)
			, spanAngle(spanAngle)
			, color(color)
			, penWidth(penWidth)
		{
		}

		QRect arc;
		int startAngle;
		int spanAngle;
		QColor color;
		int penWidth;
	};

	QList<LineCoordinates> mLines;
	QList<PixelCoordinates> mPixels;
	QList<RectCoordinates> mRects;
	QList<EllipseCoordinates> mEllipses;
	QList<ArcCoordinates> mArcs;
	//int mCurrentPenWidth;
	//QColor mCurrentPenColor;
};

}
}
}
}
