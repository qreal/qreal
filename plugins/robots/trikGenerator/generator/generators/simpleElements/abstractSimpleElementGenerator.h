#pragma once

#include "../../../../../../qrkernel/ids.h"
#include "../abstractElementGenerator.h"

namespace robots {
namespace trikGenerator {

class TrikRobotGenerator;

class AbstractSimpleElementGenerator {
public:
	AbstractSimpleElementGenerator();
	virtual ~AbstractSimpleElementGenerator();

	static QList<SmartLine> convertedCode(TrikRobotGenerator *generator
			, qReal::Id const elementId, qReal::Id const logicElementId);

protected:
	virtual QList<SmartLine> convertElementIntoDirectCommand(TrikRobotGenerator *generator
			, qReal::Id const elementId, qReal::Id const logicElementId) = 0;

	virtual QList<QString> portsToEngineNames(QString const &portsProperty) const;
	virtual QString transformSign(QString const &sign) const;
};
}
}
