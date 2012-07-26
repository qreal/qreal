#include "../../../../../../qrkernel/ids.h"
#include "abstractsimpleelementgenerator.h"
#include "../abstractElementGenerator.h"

namespace robots {
namespace generator {
class NxtOSEKRobotGenerator;

class PlayToneGenerator: public AbstractSimpleElementGenerator {
public:
	PlayToneGenerator();

protected:
	virtual QList<SmartLine> convertElementIntoDirectCommand(NxtOSEKRobotGenerator *nxtGen
			, qReal::Id const elementId, qReal::Id const logicElementId);
};
}
}
