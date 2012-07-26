#include "../../../../../../qrkernel/ids.h"
#include "abstractsimpleelementgenerator.h"
#include "../abstractElementGenerator.h"

namespace robots {
namespace generator {
class NxtOSEKRobotGenerator;

class WaitForColorBlockGenerator: public AbstractSimpleElementGenerator {
public:
	WaitForColorBlockGenerator();

protected:
	virtual QList<SmartLine> convertElementIntoDirectCommand(NxtOSEKRobotGenerator *nxtGen
			, qReal::Id const elementId, qReal::Id const logicElementId);
	virtual void addInitAndTerminateCode(NxtOSEKRobotGenerator *nxtGen,
			QString const &port, QString const &colorNxtType, qReal::Id const elementId);
};
}
}
