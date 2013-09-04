#include "waitForGyroscopeBlockGenerator.h"

using namespace qReal::robots::generators::simple;

WaitForGyroscopeBlockGenerator::WaitForGyroscopeBlockGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "waitGyroscope.t", QList<Binding *>(), parent)
{
}

//QList<SmartLine_old> WaitForGyroscopeBlockGenerator::convertElementIntoDirectCommand(NxtOSEKRobotGenerator *nxtGen
//		, qReal::Id const &elementId, qReal::Id const &logicElementId)
//{
//	QList<SmartLine_old> result;

//	QString const port = "NXT_PORT_S" + nxtGen->api()->stringProperty(logicElementId, "Port");

//	QString const degrees = nxtGen->intExpression(logicElementId, "Degrees");
//	QString const inequalitySign = transformSign(QString(nxtGen->api()->stringProperty(logicElementId
//			, "Sign").toUtf8()));

//	QString const condition = inequalitySign + " " + degrees;

//	result.append(SmartLine_old("while (!(ecrobot_get_gyro_sensor(" + port
//			+ ") " + condition + ")) {", elementId));
//	result.append(SmartLine_old("}", elementId));

//	return result;
//}
