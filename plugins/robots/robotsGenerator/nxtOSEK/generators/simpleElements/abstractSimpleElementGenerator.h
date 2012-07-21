#pragma once

#include "../../../../../../qrkernel/ids.h"
#include "../abstractElementGenerator.h"

namespace robots {
namespace generator {
class NxtOSEKRobotGenerator;

class AbstractSimpleElementGenerator {
public:
	AbstractSimpleElementGenerator();
	virtual ~AbstractSimpleElementGenerator();

	static QList<SmartLine> convertedCode(NxtOSEKRobotGenerator *nxtGen
			, qReal::Id const elementId, qReal::Id const logicElementId);

protected:
	virtual QList<SmartLine> convertElememtIntoDirectCommand(NxtOSEKRobotGenerator *nxtGen
			, qReal::Id const elementId, qReal::Id const logicElementId) = 0;
	virtual QList<QString> portsToEngineNames(QString const &portsProperty);
	virtual QString transformSign(QString const &sign);
};
}
}
