#include "waitForEncoderBlockGenerator.h"

using namespace qReal::robots::generators::simple;

WaitForEncoderBlockGenerator::WaitForEncoderBlockGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "waitEncoder.t", QList<Binding *>(), parent)
{
}

//QList<SmartLine_old> WaitForEncoderBlockGenerator::convertElementIntoDirectCommand(NxtOSEKRobotGenerator *nxtGen
//		, qReal::Id const &elementId, qReal::Id const &logicElementId)
//{
//	QList<SmartLine_old> result;

//	QString const port = nxtGen->api()->stringProperty(logicElementId, "Port");
//	QString const tachoLimit = nxtGen->api()->stringProperty(logicElementId, "TachoLimit");
//	result.append(SmartLine_old("while (nxt_motor_get_count(NXT_PORT_" + port + ") < " + tachoLimit + ") {"
//			, elementId));
//	result.append(SmartLine_old("}", elementId));

//	return result;
//}
