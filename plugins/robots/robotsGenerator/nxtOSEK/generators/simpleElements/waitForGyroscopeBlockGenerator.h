#pragma once

#include "../../../../../../qrkernel/ids.h"
#include "abstractSimpleElementGenerator.h"
#include "../abstractElementGenerator.h"

namespace robots {
namespace generator {
class NxtOSEKRobotGenerator;

class WaitForGyroscopeBlockGenerator : public AbstractSimpleElementGenerator
{
public:
	WaitForGyroscopeBlockGenerator();

protected:
	virtual QList<SmartLine> convertElementIntoDirectCommand(NxtOSEKRobotGenerator *nxtGen
			, qReal::Id const &elementId, qReal::Id const &logicElementId);

private:
	void addInitAndTerminateCode(NxtOSEKRobotGenerator *nxtGen
			, QString const &port, qReal::Id const &elementId);
};

}
}
