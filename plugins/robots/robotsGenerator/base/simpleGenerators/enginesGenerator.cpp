#include "enginesGenerator.h"
#include "../generatorCustomizer.h"

using namespace qReal::robots::generators::simple;

EnginesGenerator::EnginesGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, Id const &id
		, QString const &engineType
		, QObject *parent)
	: BindingGenerator(repo, customizer, id
			, engineType == "EnginesBackward"
					? "engines/enginesBackward.t"
					: "engines/enginesForward.t"
			, QList<Binding *>()
					<< Binding::createMultiTarget("@@PORT@@", "Ports", customizer.factory()->enginesConverter())
					<< Binding::createConverting("@@POWER@@", "Power", customizer.factory()->intPropertyConverter())
					<< Binding::createConverting("@@BREAK_MODE@@", "Mode", customizer.factory()->breakModeConverter())
			, parent)
{
}

//void EnginesGenerator::addInitAndTerminateCode(NxtOSEKRobotGenerator *nxtGen  // duplicated code - not true
//		, qReal::Id const &elementId, QString const &enginePort)
//{
//	QString const initTerminateVelocity = "0";
//	QString const initTerminateBrakeMode = "1";
//	QString const initCodeEngines = "nxt_motor_set_speed("
//			+ enginePort + ", "
//			+ initTerminateVelocity + ", "
//			+ initTerminateBrakeMode + ");";
//	QString const terminateCodeEngines = "nxt_motor_set_speed("
//			+ enginePort + ", "
//			+ initTerminateVelocity + ", "
//			+ initTerminateBrakeMode + ");";
//	if (!ListSmartLine::isContains(nxtGen->initCode(), initCodeEngines)) {
//		nxtGen->initCode().append(SmartLine_old(initCodeEngines, elementId));
//		nxtGen->terminateCode().append(SmartLine_old(terminateCodeEngines, elementId));
//	}
//}
