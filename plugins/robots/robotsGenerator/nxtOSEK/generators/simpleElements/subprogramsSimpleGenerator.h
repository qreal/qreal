#pragma once

#include "../../../../../../qrkernel/ids.h"
#include "abstractSimpleElementGenerator.h"
#include "../abstractElementGenerator.h"
#include "../../../../../../qrutils/nameNormalizer.h"

namespace robots {
namespace generator {
class NxtOSEKRobotGenerator;

class SubprogramsSimpleGenerator : public AbstractSimpleElementGenerator
{
public:
	virtual QList<SmartLine>  convertElementIntoDirectCommand(NxtOSEKRobotGenerator *nxtGen
			, qReal::Id const &elementId, qReal::Id const &logicElementId);

	static QString identificator(NxtOSEKRobotGenerator const *nxtGen, qReal::Id const &id);
};

}
}
