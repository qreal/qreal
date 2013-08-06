#include "initialNodeGenerator.h"
#include "../../trikRobotGenerator.h"

using namespace robots::trikGenerator;

InitialNodeGenerator::InitialNodeGenerator()
{
}

QList<SmartLine> InitialNodeGenerator::convertElementIntoDirectCommand(TrikRobotGenerator *trikGenerator
		, qReal::Id const elementId, qReal::Id const logicElementId)
{
	Q_UNUSED(trikGenerator)
	Q_UNUSED(elementId)
	Q_UNUSED(logicElementId)

	return QList<SmartLine>();
}
