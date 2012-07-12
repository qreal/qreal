#pragma once

#include "../../../../../qrkernel/ids.h"
#include "../smartLine.h"
#include "abstractElementGenerator.h"

namespace robots {
namespace generator {

/// Realization of AbstractElementGenerator for Beep, Engines etc.
class SimpleElementGenerator: public AbstractElementGenerator {
public:
	explicit SimpleElementGenerator(NxtOSEKRobotGenerator *mainGenerator, qReal::Id elementId);

protected:
	virtual QList<SmartLine> loopPrefixCode();

	virtual QList<SmartLine> loopPostfixCode();

	virtual bool preGenerationCheck();
	virtual bool nextElementsGeneration();

	virtual QList<SmartLine> simpleCode();

	QList<QString> portsToEngineNames(QString const &portsProperty);

private:
	QString transformSign(QString const &inequalitySign);
	QList<SmartLine> mInitCode;
};

}
}
