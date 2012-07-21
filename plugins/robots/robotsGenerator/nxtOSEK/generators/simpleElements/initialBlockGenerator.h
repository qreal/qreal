#include "../../../../../../qrkernel/ids.h"
#include "abstractsimpleelementgenerator.h"
#include "../abstractElementGenerator.h"

namespace robots {
namespace generator {
class NxtOSEKRobotGenerator;

class InitialBlockGenerator: public AbstractSimpleElementGenerator {
public:
	InitialBlockGenerator();

protected:
	virtual QList<SmartLine> convertElememtIntoDirectCommand(NxtOSEKRobotGenerator *nxtGen
			, qReal::Id const elementId, qReal::Id const logicElementId);

private:
	QList<SmartLine> mInitCode;
	QList<SmartLine> mTerminateCode;
};
}
}
