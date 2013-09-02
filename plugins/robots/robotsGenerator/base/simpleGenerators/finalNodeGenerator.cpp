#include "finalNodeGenerator.h"

using namespace qReal::robots::generators::simple;

FinalNodeGenerator::FinalNodeGenerator(LogicalModelAssistInterface const &model
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent)
	: BindingGenerator(model, customizer, id, "finalNode.t", QList<Binding *>(), parent)
{
}

//QList<SmartLine_old> FinalNodeGenerator::convertElementIntoDirectCommand(NxtOSEKRobotGenerator *nxtGen
//		, qReal::Id const &elementId, qReal::Id const &logicElementId)
//{
//	Q_UNUSED(nxtGen)
//	Q_UNUSED(logicElementId)

//	return QList<SmartLine_old>() << SmartLine_old(nxtGen->areWeGeneratingMainTask()
//			? "TerminateTask();" : "return;", elementId);
//}
