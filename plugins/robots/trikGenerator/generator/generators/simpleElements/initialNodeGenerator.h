#include "../../../../../../qrkernel/ids.h"
#include "abstractSimpleElementGenerator.h"
#include "../abstractElementGenerator.h"

namespace robots {
namespace trikGenerator {

class TrikRobotGenerator;

class InitialNodeGenerator: public AbstractSimpleElementGenerator {
public:
	InitialNodeGenerator();

protected:
	virtual QList<SmartLine> convertElementIntoDirectCommand(TrikRobotGenerator *trikGenerator
			, qReal::Id const elementId, qReal::Id const logicElementId);

private:
	void appendColorCode(QList<SmartLine> &initCode, QList<SmartLine> &terminateCode
			, QList<SmartLine> &isrHooks, bool &foundColorSensor
			, qReal::Id const &elementId, QString const &sensor, int port);
};

}
}
