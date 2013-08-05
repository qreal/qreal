#include "../../../../../../qrkernel/ids.h"
#include "abstractSimpleElementGenerator.h"
#include "../abstractElementGenerator.h"

namespace robots {
namespace trikGenerator {

class NxtOSEKRobotGenerator;

class EnginesStopGenerator: public AbstractSimpleElementGenerator {
public:
	EnginesStopGenerator();

protected:
	virtual QList<SmartLine> convertElementIntoDirectCommand(NxtOSEKRobotGenerator *nxtGen
			, qReal::Id const elementId, qReal::Id const logicElementId);
};
}
}
