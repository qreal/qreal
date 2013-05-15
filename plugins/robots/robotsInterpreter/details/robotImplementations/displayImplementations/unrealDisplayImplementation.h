#pragma once

#include <QList>
#include <QLine>

#include "abstractDisplayImplementation.h"
#include "../../robotCommandConstants.h"
#include "../../d2RobotModel/d2RobotModel.h"
#include "../details/nxtDisplay.h"
#include "../../../../../../qrutils/graphicsUtils/painterInterface.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace robotImplementations {
namespace displayImplementations {
class UnrealDisplayImplementation : public AbstractDisplayImplementation, public graphicsUtils::PainterInterface
{
	Q_OBJECT
public:
	UnrealDisplayImplementation(d2Model::D2RobotModel *d2Model);
	virtual void read();
	virtual void drawPixel(int x, int y);
	virtual void drawLine(int x1, int y1, int x2, int y2);
	virtual void drawRect(int x, int y, int width, int height);
	virtual void drawCircle(int x, int y, int radius);
	virtual void printText(int x, int y, QString text);
	virtual void paint(QPainter *painter);
	virtual void clearScreen();
	virtual void attachToPaintWidget();

protected:
	d2Model::D2RobotModel *mD2Model;
	QList<QLine> mLines;
	QList<QPoint> mPoints;
	QList<QRect> mCircles;
	QList<QRect> mRects;
};
}
}
}
}
}
}
