#pragma once

#include <QtGui/QImage>
#include <QtGui/QPainter>

#include "../../../../../../qrkernel/ids.h"
#include "abstractSimpleElementGenerator.h"
#include "../abstractElementGenerator.h"

namespace robots {
namespace generator {

int const displayWidth = 100;
int const displayHeight = 64;

class NxtOSEKRobotGenerator;

class DrawBlockGenerator : public AbstractSimpleElementGenerator
{
public:
	DrawBlockGenerator();

protected:
	virtual QList<SmartLine> convertElementIntoDirectCommand(NxtOSEKRobotGenerator *nxtGen
			, qReal::Id const elementId, qReal::Id const logicElementId);

	virtual void generateBmpFile(NxtOSEKRobotGenerator *nxtGen, QString name, qReal::Id const logicElementId);
	virtual void drawBmp(NxtOSEKRobotGenerator *nxtGen, qReal::Id const logicElementId, QPainter *p) = 0;
};
}
}
