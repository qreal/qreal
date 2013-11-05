#pragma once

#include <QtCore/QList>
#include <QtCore/QLine>

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

int const textPixelHeight = 8;
int const textPixelWidth = 16;
int const nxtDisplayHeight = 64;
int const nxtDisplayWidth = 100;

class UnrealDisplayImplementation : public AbstractDisplayImplementation, public graphicsUtils::PainterInterface
{
	Q_OBJECT
public:
	UnrealDisplayImplementation(d2Model::D2RobotModel *d2Model);
	virtual ~UnrealDisplayImplementation() {}

	virtual void read();
	virtual void paint(QPainter *painter);

	virtual void drawImage(QImage const &image);
	virtual void setBackground(QColor const &color);
	virtual void clearScreen();

	void attachToPaintWidget();

protected:
	d2Model::D2RobotModel *mD2Model;
	QColor mBackground;
	QImage mCurrentImage;
};
}
}
}
}
}
}
