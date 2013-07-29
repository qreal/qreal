#pragma once

#include "../../../../../../qrkernel/ids.h"
#include "abstractSimpleElementGenerator.h"
#include "../abstractElementGenerator.h"
#include "readBtMessageBlockGenerator.h"

namespace robots {
namespace generator {
class NxtOSEKRobotGenerator;

class ReadBtVariableBlockGenerator : public ReadBtMessageBlockGenerator
{
public:
	ReadBtVariableBlockGenerator();

protected:
	virtual QString generateReadCommand(NxtOSEKRobotGenerator *nxtGen, qReal::Id const &logicElementId);
};
}
}
