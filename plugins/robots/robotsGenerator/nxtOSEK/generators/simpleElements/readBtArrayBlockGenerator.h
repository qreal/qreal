#pragma once

#include "../../../../../../qrkernel/ids.h"
#include "abstractSimpleElementGenerator.h"
#include "../abstractElementGenerator.h"
#include "readBtMessageBlockGenerator.h"

namespace robots {
namespace generator {
class NxtOSEKRobotGenerator;

class ReadBtArrayBlockGenerator : public ReadBtMessageBlockGenerator
{
public:
	ReadBtArrayBlockGenerator();

protected:
	virtual QString generateReadCommand(NxtOSEKRobotGenerator *nxtGen, qReal::Id const &logicElementId);
};
}
}
