#pragma once

#include "../../../../../../qrkernel/ids.h"
#include "abstractSimpleElementGenerator.h"
#include "../abstractElementGenerator.h"
#include "drawBlockGenerator.h"

namespace robots {
namespace generator {
class NxtOSEKRobotGenerator;

class DrawPixelBlockGenerator : public DrawBlockGenerator
{
public:
	DrawPixelBlockGenerator();

protected:
	virtual void drawBmp(NxtOSEKRobotGenerator *nxtGen, qReal::Id const logicElementId, QPainter *p);
};

}
}
