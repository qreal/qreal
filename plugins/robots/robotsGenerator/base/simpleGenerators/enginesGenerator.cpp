#include "enginesGenerator.h"
#include "../generatorCustomizer.h"

using namespace qReal::robots::generators::simple;

EnginesGenerator::EnginesGenerator(LogicalModelAssistInterface const &model
		, GeneratorCustomizer &customizer
		, Id const &id
		, QString const &engineType
		, QObject *parent)
	: BindingGenerator(model, customizer, id
			, engineType == "EnginesBackward" ? "enginesBackward.t" : "enginesForward.t"
			, QList<Binding *>()
					<< new Binding("@@POWER@@", "Power", customizer.factory()->intPropertyConverter())
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

//QList<SmartLine_old> EnginesGenerator::convertElementIntoDirectCommand(NxtOSEKRobotGenerator *nxtGen
//		 , qReal::Id const &elementId, qReal::Id const &logicElementId)
//{
//	QList<SmartLine_old> result;

//	QString const signRotate = mEngineType == "EnginesBackward" ? "-" : "";
//	QString const power = nxtGen->api()->stringProperty(logicElementId, "Power");
//	QString const signedPower = signRotate + power;
//	QString const intPower = nxtGen->variables().expressionToInt(signedPower);

//	QString const brakeModeRaw = nxtGen->api()->stringProperty(logicElementId, "Mode");
//	QByteArray const brakeMode = brakeModeRaw == QString::fromUtf8("скользить") ? "0" : "1";

//	foreach (QString const &enginePort, portsToEngineNames(nxtGen->api()->stringProperty(logicElementId, "Ports"))) {
//		result.append(SmartLine_old("nxt_motor_set_speed("
//				+ enginePort + ", "
//				+ intPower + ", "
//				+ brakeMode + ");", elementId));
//		addInitAndTerminateCode(nxtGen, elementId, enginePort);
//	}

//	return result;
//}
