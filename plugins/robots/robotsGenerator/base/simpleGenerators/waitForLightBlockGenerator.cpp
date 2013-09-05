#include "waitForLightBlockGenerator.h"
#include "../generatorCustomizer.h"

using namespace qReal::robots::generators::simple;

WaitForLightBlockGenerator::WaitForLightBlockGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "wait/light.t", QList<Binding *>()
			<< Binding::createConverting("@@PORT@@", "Port", customizer.factory()->inputPortConverter())
			<< Binding::createConverting("@@PERCENTS@@", "Percents", customizer.factory()->intPropertyConverter())
			<< Binding::createConverting("@@SIGN@@", "Sign", customizer.factory()->inequalitySignConverter())
			, parent)
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
