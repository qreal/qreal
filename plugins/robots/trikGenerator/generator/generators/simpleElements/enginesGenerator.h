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
	virtual QList<SmartLine> convertElementIntoDirectCommand(TrikRobotGenerator *generator
			, qReal::Id const elementId, qReal::Id const logicElementId);

private:
	QString mEngineType;

};

}
}
