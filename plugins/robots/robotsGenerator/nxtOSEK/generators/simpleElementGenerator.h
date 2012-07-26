#pragma once

#include "../../../../../qrkernel/ids.h"
#include "../smartLine.h"
#include "abstractElementGenerator.h"

namespace robots {
namespace generator {

/// Realization of AbstractElementGenerator for Beep, Engines etc.
class SimpleElementGenerator: public AbstractElementGenerator {
public:
	explicit SimpleElementGenerator(NxtOSEKRobotGenerator *mainGenerator, qReal::Id const &elementId);

protected:
	virtual QList<SmartLine> addLoopCodeInPrefixForm();
	virtual QList<SmartLine> addLoopCodeInPostfixForm();

	virtual bool preGenerationCheck();
	virtual bool nextElementsGeneration();

	virtual QList<SmartLine> convertBlockIntoCode();
};

}
}
