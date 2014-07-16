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
	//void drawPixel(int x, int y) override;
	void drawLine(int x1, int y1, int x2, int y2) override;
	//void drawRect(int x, int y, int width, int height)  override;
	//void drawEllipse(int x, int y, int width, int height) override;
	//void drawArc(int x, int y, int width, int height, int startAngle, int spanAngle) override;

	void paint(QPainter *painter) override;
	void clear() override;

private:
	twoDModel::engine::TwoDModelEngineInterface &mEngine;
	QColor mBackground;
	QImage mCurrentImage;
	/// @todo: QPoint can`t be used in map without operators declaration.
	QHash<QPair<int, int>, QString> mLabels;

	struct LineCoordinates
	{
		LineCoordinates(int x1, int y1, int x2, int y2, QColor color, int penWidth)
			: coord1(QPoint(x1, y1)), coord2(QPoint(x2, y2)), color(color), penWidth(penWidth)
		{
		}

		QPoint coord1;
		QPoint coord2;
		QColor color;
		int penWidth;
	};

	QList<LineCoordinates> mLines;
	//int mCurrentPenWidth;
	//QColor mCurrentPenColor;
};

}
}
}
}
