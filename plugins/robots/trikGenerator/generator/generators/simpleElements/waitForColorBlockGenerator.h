#include "../../../../../../qrkernel/ids.h"
#include "abstractSimpleElementGenerator.h"
#include "../abstractElementGenerator.h"

namespace robots {
namespace trikGenerator {

class NxtOSEKRobotGenerator;

class WaitForColorBlockGenerator: public AbstractSimpleElementGenerator {
public:
	WaitForColorBlockGenerator();

protected:
	virtual QList<SmartLine> convertElementIntoDirectCommand(NxtOSEKRobotGenerator *nxtGen
			, qReal::Id const elementId, qReal::Id const logicElementId);

private:
	void addInitAndTerminateCode(NxtOSEKRobotGenerator *nxtGen,
			QString const &port, QString const &colorNxtType, qReal::Id const elementId);
};
}
}
