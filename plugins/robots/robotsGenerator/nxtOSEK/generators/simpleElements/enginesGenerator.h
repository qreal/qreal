#pragma once

#include "../../../../../../qrkernel/ids.h"
#include "abstractSimpleElementGenerator.h"
#include "../abstractElementGenerator.h"

namespace robots {
namespace generator {
class NxtOSEKRobotGenerator;

class EnginesGenerator : public AbstractSimpleElementGenerator
{
public:
	EnginesGenerator(QString const &engineType);

protected:
	virtual QList<SmartLine_old> convertElementIntoDirectCommand(NxtOSEKRobotGenerator *nxtGen
			, qReal::Id const &elementId, qReal::Id const &logicElementId);

private:
	void addInitAndTerminateCode(NxtOSEKRobotGenerator *nxtGen
			, qReal::Id const &elementId, QString const &enginePort);
	QString mEngineType;

};

}
}
