#pragma once

#include "../../../../../qrkernel/ids.h"
#include "../smartLine.h"
#include "../nxtOSEKRobotGenerator.h"
#include "simpleElementGenerator.h"

namespace robots {
namespace generator {

/// Realization of AbstractElementGenerator for Function.
class FunctionElementGenerator: public SimpleElementGenerator {
public:
	explicit FunctionElementGenerator(NxtOSEKRobotGenerator *generator
			, qReal::Id const &elementId
			, bool const &generateToInit);

protected:
	virtual QList<SmartLine> convertBlockIntoCode();
	void variableAnalysis(QByteArray const &code);

private:
	QByteArray replaceSensorVariables(QByteArray portValue) const;
	bool mGenerateToInit;
};

}
}
