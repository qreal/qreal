#include "initialNodeGenerator.h"
#include "../../trikRobotGenerator.h"

using namespace robots::trikGenerator;

InitialNodeGenerator::InitialNodeGenerator()
{
}

QList<SmartLine> InitialNodeGenerator::convertElementIntoDirectCommand(TrikRobotGenerator *generator
		, qReal::Id const elementId, qReal::Id const logicElementId)
{
	Q_UNUSED(generator)
	Q_UNUSED(elementId)
	Q_UNUSED(logicElementId)

	return QList<SmartLine>();
}
