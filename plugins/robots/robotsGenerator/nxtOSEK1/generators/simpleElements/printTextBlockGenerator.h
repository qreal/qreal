#pragma once

#include "../../../../../../qrkernel/ids.h"
#include "abstractSimpleElementGenerator.h"
#include "../abstractElementGenerator.h"

namespace robots {
namespace generator {
class NxtOSEKRobotGenerator;

class PrintTextBlockGenerator : public AbstractSimpleElementGenerator
{
public:
	PrintTextBlockGenerator();

protected:
	virtual QList<SmartLine_old> convertElementIntoDirectCommand(NxtOSEKRobotGenerator *nxtGen
			, qReal::Id const &elementId, qReal::Id const &logicElementId);
};

}
}
