#include "waitForSonarBlockGenerator.h"
#include "../generatorCustomizer.h"

using namespace qReal::robots::generators::simple;

// TODO: make it customizable
int const sleepInterval = 10;

WaitForSonarBlockGenerator::WaitForSonarBlockGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "wait/sonar.t", QList<Binding *>()
			<< Binding::createConverting("@@PORT@@", "Port", customizer.factory()->inputPortConverter())
			<< Binding::createConverting("@@DISTANCE@@", "Distance", customizer.factory()->intPropertyConverter())
			<< Binding::createConverting("@@SIGN@@", "Sign", customizer.factory()->inequalitySignConverter())
			, parent)
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
