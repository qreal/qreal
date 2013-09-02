#include "finalNodeGenerator.h"

using namespace qReal::robots::generators::simple;

FinalNodeGenerator::FinalNodeGenerator()
{
}

QList<SmartLine_old> FinalNodeGenerator::convertElementIntoDirectCommand(NxtOSEKRobotGenerator *nxtGen
		, qReal::Id const &elementId, qReal::Id const &logicElementId)
{
	Q_UNUSED(nxtGen)
	Q_UNUSED(logicElementId)

	return QList<SmartLine_old>() << SmartLine_old(nxtGen->areWeGeneratingMainTask()
			? "TerminateTask();" : "return;", elementId);
}
