#include "waitForSonarBlockGenerator.h"

using namespace qReal::robots::generators::simple;

// TODO: make it customizable
int const sleepInterval = 10;

WaitForSonarBlockGenerator::WaitForSonarBlockGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "waitSonar.t", QList<Binding *>(), parent)
{
}

//void WaitForSonarBlockGenerator::addInitAndTerminateCode(NxtOSEKRobotGenerator *nxtGen
//			, QString const &port, qReal::Id const &elementId)
//{
//	QString const initCode = "ecrobot_init_sonar_sensor(" + port + ");";
//	if (!ListSmartLine::isContains(nxtGen->initCode(), initCode)) {
//		QString const terminateCode = "ecrobot_term_sonar_sensor(" + port + ");";
//		nxtGen->initCode().append(SmartLine_old(initCode, elementId));
//		nxtGen->terminateCode().append(SmartLine_old(terminateCode, elementId));
//	}
//}
//QList<SmartLine_old> WaitForSonarBlockGenerator::convertElementIntoDirectCommand(NxtOSEKRobotGenerator *nxtGen
//		, qReal::Id const &elementId, qReal::Id const &logicElementId)
//{
//	QList<SmartLine_old> result;

//	QString const port = "NXT_PORT_S" + nxtGen->api()->stringProperty(logicElementId, "Port");
//	QString const distance = nxtGen->api()->stringProperty(logicElementId, "Distance");
//	QString const inequalitySign = transformSign(QString(nxtGen->api()->stringProperty(logicElementId
//			, "Sign").toUtf8()));
//	QString const condition = inequalitySign + " " + distance;

//	result.append(SmartLine_old("while (!(ecrobot_get_sonar_sensor(" + port + ") "
//			+ condition + ")) {", elementId, SmartLine_old::increase));
//	result.append(SmartLine_old(QString("systick_wait_ms(%1);").arg(
//			QString::number(sleepInterval)), elementId));
//	result.append(SmartLine_old("}", elementId, SmartLine_old::decrease));

//	addInitAndTerminateCode(nxtGen, port, elementId);

//	return result;
//}
