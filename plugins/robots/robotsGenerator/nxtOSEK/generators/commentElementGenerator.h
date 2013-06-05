#pragma once

#include "../../../../../qrkernel/ids.h"

#include "../smartLine.h"
#include "abstractElementGenerator.h"

namespace robots {
namespace generator {

/// Implementation of AbstractElementGenerator for Comment.
class CommentElementGenerator : public AbstractElementGenerator
{
public:
	CommentElementGenerator(NxtOSEKRobotGenerator *gen, qReal::Id const &elementId);

protected:
	virtual QList<SmartLine> addLoopCodeInPrefixForm();
	virtual QList<SmartLine> addLoopCodeInPostfixForm();

	virtual bool preGenerationCheck();
	virtual bool nextElementsGeneration();

};

}
}
