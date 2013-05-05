#pragma once

#include "../../../../../qrkernel/ids.h"
#include "../smartLine.h"
#include "../../../robotsInterpreter/sensorConstants.h"

namespace robots {
namespace generator {

class NxtOSEKRobotGenerator;

/// AbstractElementGenerator - robot diagram element generator abstraction.
class AbstractElementGenerator {
public:
	explicit AbstractElementGenerator(NxtOSEKRobotGenerator *mainGenerator, qReal::Id const &elementId);
	virtual ~AbstractElementGenerator();
	virtual bool generate();

protected:
	virtual QList<SmartLine> addLoopCodeInPrefixForm() = 0;
	virtual QList<SmartLine> addLoopCodeInPostfixForm() = 0;

	/**
	 * Creates new lists in mGeneratedStringSet
	 * and connects it with mElementId in mElementToStringListNumbers
	 * in case element have more than 1 incoming connection.
	*/
	virtual void createListsForIncomingConnections();
	virtual bool preGenerationCheck() = 0;
	virtual bool nextElementsGeneration() = 0;

	void replaceSensorAndEncoderVariables(QString &target);

	NxtOSEKRobotGenerator *mNxtGen;
	qReal::Id mElementId;

private:
	QString replaceSensorVariables(qReal::interpreters::robots::sensorType::SensorTypeEnum portValue) const;
	QString replaceEncoderVariables() const;
};

}
}
