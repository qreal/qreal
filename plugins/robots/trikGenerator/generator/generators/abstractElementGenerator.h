#pragma once

#include "../../../../../qrkernel/ids.h"
#include "../smartLine.h"
#include "../../../robotsInterpreter/sensorConstants.h"

namespace robots {
namespace trikGenerator {

class TrikRobotGenerator;

/// AbstractElementGenerator - robot diagram element generator abstraction.
class AbstractElementGenerator
{
public:
	explicit AbstractElementGenerator(TrikRobotGenerator *mainGenerator, qReal::Id const &elementId);
	virtual ~AbstractElementGenerator();
	virtual bool generate();

protected:
	virtual QList<SmartLine> addLoopCodeInPrefixForm() = 0;
	virtual QList<SmartLine> addLoopCodeInPostfixForm() = 0;

	/// Creates new lists in mGeneratedStringSet
	/// and connects it with mElementId in mElementToStringListNumbers
	/// in case element have more than 1 incoming connection.
	virtual void createListsForIncomingConnections();

	virtual bool preGenerationCheck() = 0;
	virtual bool nextElementsGeneration() = 0;

	/// Replaces all sensor and encoder variables occurences with corresponding
	/// nxtOSEK API expression.
	QString replaceSensorAndEncoderVariables(QString const &expression) const;

	/// Replaces function invocations to calls of corresponding functions in C.
	QString replaceFunctionInvocations(QString const &expression) const;

	TrikRobotGenerator *mTrikGenerator;
	qReal::Id mElementId;

private:
	QString sensorExpression(int port) const;
	QString encoderExpression() const;
};

}
}
