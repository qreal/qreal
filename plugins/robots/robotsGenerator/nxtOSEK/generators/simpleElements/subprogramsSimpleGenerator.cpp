#include "subprogramsSimpleGenerator.h"
#include "../../nxtOSEKRobotGenerator.h"

using namespace robots::generator;

QList<SmartLine> SubprogramsSimpleGenerator::convertElementIntoDirectCommand(NxtOSEKRobotGenerator *nxtGen
		, qReal::Id const &elementId, qReal::Id const &logicElementId)
{
	nxtGen->subprogramsGenerator().usageFound(logicElementId);
	return QList<SmartLine>() << SmartLine(identificator(nxtGen, elementId) + "();", elementId);
}

QString SubprogramsSimpleGenerator::identificator(NxtOSEKRobotGenerator const *nxtGen, qReal::Id const &id)
{
	QString const rawName = nxtGen->api()->name(id);
	return utils::NameNormalizer::normalizeStrongly(rawName, false);
}
