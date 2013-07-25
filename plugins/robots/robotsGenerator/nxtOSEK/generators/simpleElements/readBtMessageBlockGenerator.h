#pragma once

#include "../../../../../../qrkernel/ids.h"
#include "abstractSimpleElementGenerator.h"
#include "../abstractElementGenerator.h"

namespace robots {
namespace generator {
class NxtOSEKRobotGenerator;

class ReadBtMessageBlockGenerator : public AbstractSimpleElementGenerator
{
public:
	ReadBtMessageBlockGenerator();

protected:
	virtual QList<SmartLine> convertElementIntoDirectCommand(NxtOSEKRobotGenerator *nxtGen
			, qReal::Id const &elementId, qReal::Id const &logicElementId);
	virtual QString generateReadCommand(NxtOSEKRobotGenerator *nxtGen, qReal::Id const &logicElementId) = 0;
};
}
}
