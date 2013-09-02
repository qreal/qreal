#pragma once

#include "../../../../../qrkernel/ids.h"
#include "../smartLine.h"
#include "abstractElementGenerator.h"

namespace robots {
namespace generator {

/// Implementation of AbstractElementGenerator for Beep, Engines etc.
class SimpleElementGenerator: public AbstractElementGenerator
{
public:
	SimpleElementGenerator(NxtOSEKRobotGenerator *mainGenerator, qReal::Id const &elementId);

protected:
	virtual QList<SmartLine_old> addLoopCodeInPrefixForm();
	virtual QList<SmartLine_old> addLoopCodeInPostfixForm();

	virtual bool preGenerationCheck();
	virtual bool nextElementsGeneration();

	virtual QList<SmartLine_old> convertBlockIntoCode();
};

}
}
