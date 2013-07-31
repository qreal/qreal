#include "finalNodeGenerator.h"
#include "../../trikRobotGenerator.h"

using namespace robots::trikGenerator;

FinalNodeGenerator::FinalNodeGenerator()
{
}

QList<SmartLine> FinalNodeGenerator::convertElementIntoDirectCommand(TrikRobotGenerator *generator
		, qReal::Id const elementId, qReal::Id const logicElementId)
{
	Q_UNUSED(generator)
	Q_UNUSED(logicElementId)

	QList<SmartLine> result;
	result.append(SmartLine("brick.stop();", elementId));

	return result;
}
