#pragma once

#include "../../../../../qrkernel/ids.h"
#include "../smartLine.h"
#include "abstractElementGenerator.h"

namespace robots {
namespace generator {

/// Realization of AbstractElementGenerator for Loop.
class LoopElementGenerator: public AbstractElementGenerator {
public:
	explicit LoopElementGenerator(NxtOSEKRobotGenerator *emboxGen, qReal::Id elementId);

protected:
	virtual QList<SmartLine> loopPrefixCode();

	virtual QList<SmartLine> loopPostfixCode();

	virtual bool preGenerationCheck();
	virtual bool nextElementsGeneration();
};

}
}
