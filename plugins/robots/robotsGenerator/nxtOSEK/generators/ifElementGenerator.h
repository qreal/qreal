#pragma once

#include "../../../../../qrkernel/ids.h"

#include "../smartLine.h"
#include "abstractElementGenerator.h"

namespace robots {
namespace generator {

/// Realization of AbstractElementGenerator for If block.
class IfElementGenerator : public AbstractElementGenerator {
public:
	explicit IfElementGenerator(NxtOSEKRobotGenerator *emboxGen, qReal::Id elementId);

protected:
	virtual QList<SmartLine> loopPrefixCode();

	virtual QList<SmartLine> loopPostfixCode();

	virtual bool preGenerationCheck();
	virtual bool nextElementsGeneration();

	bool generateBranch(int branchNumber);

	QPair<bool, qReal::Id> checkBranchForBackArrows(qReal::Id const &curElementId); //initial step of checking
	QPair<bool, qReal::Id> checkBranchForBackArrows(qReal::Id const &curElementId, qReal::IdList* checkedElements);
};

}
}
