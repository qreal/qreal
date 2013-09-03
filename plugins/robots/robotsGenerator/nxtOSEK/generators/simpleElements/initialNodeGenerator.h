#pragma once

#include "../../../../../../qrkernel/ids.h"
#include "abstractSimpleElementGenerator.h"
#include "../abstractElementGenerator.h"

namespace robots {
namespace generator {
class NxtOSEKRobotGenerator;

class InitialNodeGenerator : public AbstractSimpleElementGenerator
{
public:
	InitialNodeGenerator();

protected:
	virtual QList<SmartLine> convertElementIntoDirectCommand(NxtOSEKRobotGenerator *nxtGen
			, qReal::Id const &elementId, qReal::Id const &logicElementId);

private:
	void appendColorCode(QList<SmartLine> &initCode, QList<SmartLine> &terminateCode
			, QList<SmartLine> &isrHooks, bool &foundColorSensor
			, qReal::Id const &elementId, QString const &sensor, int port);
};

}
}
