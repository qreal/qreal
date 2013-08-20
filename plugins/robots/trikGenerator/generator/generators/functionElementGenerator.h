#pragma once

#include "../../../../../qrkernel/ids.h"
#include "../smartLine.h"
#include "../trikRobotGenerator.h"
#include "simpleElementGenerator.h"

namespace robots {
namespace trikGenerator {

/// Implementation of AbstractElementGenerator for Function.
class FunctionElementGenerator : public SimpleElementGenerator
{
public:
	explicit FunctionElementGenerator(TrikRobotGenerator *generator
			, qReal::Id const &elementId
			, bool const &generateToInit);

protected:
	virtual QList<SmartLine> convertBlockIntoCode();

private:
	bool mGenerateToInit;
};

}
}
