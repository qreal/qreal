#include "../../../../../../qrkernel/ids.h"
#include "abstractSimpleElementGenerator.h"
#include "../abstractElementGenerator.h"

namespace robots {
namespace trikGenerator {

class TrikRobotGenerator;

class EnginesGenerator: public AbstractSimpleElementGenerator {
public:
	EnginesGenerator(QString const &engineType);

protected:
	virtual QList<SmartLine> convertElementIntoDirectCommand(TrikRobotGenerator *nxtGen
			, qReal::Id const elementId, qReal::Id const logicElementId);

private:
	void addInitAndTerminateCode(TrikRobotGenerator *nxtGen
			, qReal::Id const elementId, QString const &enginePort);
	QString mEngineType;

};
}
}
