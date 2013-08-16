#pragma once

#include "../../../../../qrkernel/ids.h"
#include "../smartLine.h"
#include "abstractElementGenerator.h"

namespace robots {
namespace generator {

/// Implementation of AbstractElementGenerator for Loop.
class LoopElementGenerator : public AbstractElementGenerator
{
public:
	LoopElementGenerator(NxtOSEKRobotGenerator *gen, qReal::Id const &elementId);

protected:
	virtual QList<SmartLine_old> addLoopCodeInPrefixForm();
	virtual QList<SmartLine_old> addLoopCodeInPostfixForm();

	virtual bool preGenerationCheck();
	virtual bool nextElementsGeneration();
};

}
}
