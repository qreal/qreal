#pragma once

#include "../../../../../qrkernel/ids.h"

#include "../smartLine.h"
#include "abstractElementGenerator.h"

namespace robots {
namespace generator {

/// Realization of AbstractElementGenerator for If block.
class IfElementGenerator : public AbstractElementGenerator {
public:
	explicit IfElementGenerator(NxtOSEKRobotGenerator *emboxGen, qReal::Id const &elementId);

protected:
	virtual QList<SmartLine> addLoopCodeInPrefixForm();
	virtual QList<SmartLine> addLoopCodeInPostfixForm();

	virtual bool preGenerationCheck();
	virtual bool nextElementsGeneration();

	bool generateBranch(int branchNumber);

	QPair<bool, qReal::Id> checkBranchForBackArrows(qReal::Id const &curElementId); //initial step of checking
	QPair<bool, qReal::Id> checkBranchForBackArrows(qReal::Id const &curElementId, qReal::IdList* checkedElements);

private:
	virtual void displaysSuitableError(QPair<bool, qReal::Id> const positiveBranchCheck
			, QPair<bool, qReal::Id> const negativeBranchCheck);
	virtual void generateIfBlock(bool isPositiveBranchReturnsToBackElems, int const conditionArrowNum, QString condition);
	virtual void generateBlockIfElseIs(QString condition, int conditionArrowNum);
	virtual void addNeededCondition(QString &condition, qReal::IdList outgoingLinks, int conditionArrowNum);

	/// Checks that positive and negative branches are correct and adds error if not.
	/// @returns true, if everything is ok
	bool areOutgoingLinksCorrect(
			qReal::Id const positiveBranchGraphicalId
			, qReal::Id const negativeBranchGraphicalId
			, bool &isPositiveBranchReturnsToBackElems
			, bool &isNegativeBranchReturnsToBackElems
			);
};

}
}
