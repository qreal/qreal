#pragma once

#include "drawBlockGenerator.h"

namespace qReal {
namespace robots {
namespace generators {
namespace simple {

class DrawLineBlockGenerator : public DrawBlockGenerator
{
public:
	DrawLineBlockGenerator(LogicalModelAssistInterface const &model
			, GeneratorCustomizer &customizer
			, Id const &id
			, QObject *parent = 0);

protected:
	virtual void drawBmp(NxtOSEKRobotGenerator *nxtGen, qReal::Id const logicElementId, QPainter *p);
};

}
}
}
}
