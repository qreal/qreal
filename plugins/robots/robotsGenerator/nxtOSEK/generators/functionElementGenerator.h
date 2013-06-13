#pragma once

#include "../../../../../qrkernel/ids.h"
#include "../smartLine.h"
#include "../nxtOSEKRobotGenerator.h"
#include "simpleElementGenerator.h"

namespace robots {
namespace generator {

/// Implementation of AbstractElementGenerator for Function.
class FunctionElementGenerator : public SimpleElementGenerator
{
public:
	explicit FunctionElementGenerator(NxtOSEKRobotGenerator *generator
			, qReal::Id const &elementId
			, bool const &generateToInit);

protected:
	virtual QList<SmartLine> convertBlockIntoCode();

private:
	bool mGenerateToInit;
};

}
}
