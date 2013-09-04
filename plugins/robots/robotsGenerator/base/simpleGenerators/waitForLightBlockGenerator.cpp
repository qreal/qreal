#include "waitForLightBlockGenerator.h"

using namespace qReal::robots::generators::simple;

WaitForLightBlockGenerator::WaitForLightBlockGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "waitLight.t", QList<Binding *>(), parent)
{
}

//void WaitForLightBlockGenerator::addInitAndTerminateCode(NxtOSEKRobotGenerator *nxtGen
//			, QString const &port, qReal::Id const &elementId)
//{
//	QString const initCode = "ecrobot_set_light_sensor_active(" + port + ");";
//	if (!ListSmartLine::isContains(nxtGen->initCode(), initCode)) {
//		QString const terminateCode = "ecrobot_set_light_sensor_inactive(" + port + ");";
//		nxtGen->initCode().append(SmartLine_old(initCode, elementId));
//		nxtGen->terminateCode().append(SmartLine_old(terminateCode, elementId));
//	}
//}

//QList<SmartLine_old> WaitForLightBlockGenerator::convertElementIntoDirectCommand(NxtOSEKRobotGenerator *nxtGen
//		, qReal::Id const &elementId, qReal::Id const &logicElementId)
//{
//	QList<SmartLine_old> result;

//	QString const port = "NXT_PORT_S" + nxtGen->api()->stringProperty(logicElementId, "Port");

//	QString const percents = nxtGen->intExpression(logicElementId, "Percents");
//	QString const inequalitySign = transformSign(QString(nxtGen->api()->stringProperty(logicElementId
//			, "Sign").toUtf8()));

//	QString const condition = inequalitySign + " " + percents;

//	result.append(SmartLine_old("while (!(ecrobot_get_light_sensor(" + port
//			+ ") * 100 / 1023 " + condition + ")) {", elementId));
//	result.append(SmartLine_old("}", elementId));

//	addInitAndTerminateCode(nxtGen, port, elementId);
//	return result;
//}
