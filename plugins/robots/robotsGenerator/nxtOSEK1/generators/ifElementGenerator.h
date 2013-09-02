#pragma once

#include "../../../../../qrkernel/ids.h"

#include "../smartLine.h"
#include "abstractElementGenerator.h"

namespace robots {
namespace generator {

/// Implementation of AbstractElementGenerator for If block.
class IfElementGenerator : public AbstractElementGenerator
{
public:
	IfElementGenerator(NxtOSEKRobotGenerator *emboxGen, qReal::Id const &elementId);

protected:
	virtual QList<SmartLine_old> addLoopCodeInPrefixForm();
	virtual QList<SmartLine_old> addLoopCodeInPostfixForm();

	virtual bool preGenerationCheck();
	virtual bool nextElementsGeneration();

	bool generateBranch(qReal::Id const &branchElement);

	QPair<bool, qReal::Id> checkBranchForBackArrows(qReal::Id const &curElementId); //initial step of checking
	QPair<bool, qReal::Id> checkBranchForBackArrows(qReal::Id const &curElementId, qReal::IdList* checkedElements);

private:
	void displaysSuitableError(QPair<bool, qReal::Id> const positiveBranchCheck
			, QPair<bool, qReal::Id> const negativeBranchCheck);
	void generateIfBlock(bool isPositiveBranchReturnsToBackElems, QString const &condition);
	void generateBlockIfElseIs(QString const &condition);

	/// Checks that positive and negative branches are correct and adds error if not.
	/// @returns true, if everything is ok
	bool areOutgoingLinksCorrect(
			bool &isPositiveBranchReturnsToBackElems
			, bool &isNegativeBranchReturnsToBackElems
			);

	qReal::Id mTrueId;
	qReal::Id mFalseId;
};

}
}
