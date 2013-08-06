#include "../../../../../../qrkernel/ids.h"
#include "abstractSimpleElementGenerator.h"
#include "../abstractElementGenerator.h"

namespace robots {
namespace trikGenerator {

class TrikRobotGenerator;

class TimerGenerator: public AbstractSimpleElementGenerator {
public:
	TimerGenerator();

protected:
	virtual QList<SmartLine> convertElementIntoDirectCommand(TrikRobotGenerator *nxtGen
			, qReal::Id const elementId, qReal::Id const logicElementId);
};
}
}
