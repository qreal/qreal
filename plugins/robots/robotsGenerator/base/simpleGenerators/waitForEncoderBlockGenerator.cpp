#include "waitForEncoderBlockGenerator.h"
#include "../generatorCustomizer.h"

using namespace qReal::robots::generators::simple;

WaitForEncoderBlockGenerator::WaitForEncoderBlockGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "wait/encoder.t", QList<Binding *>()
			<< Binding::createConverting("@@PORT@@", "Port", customizer.factory()->outputPortConverter())
			<< Binding::createConverting("@@TACHO_LIMIT@@", "TachoLimit", customizer.factory()->intPropertyConverter())
			<< Binding::createConverting("@@SIGN@@", "Sign", customizer.factory()->inequalitySignConverter())
			, parent)
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
