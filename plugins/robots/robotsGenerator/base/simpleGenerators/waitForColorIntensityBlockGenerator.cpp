#include "waitForColorIntensityBlockGenerator.h"
#include "../generatorCustomizer.h"

using namespace qReal::robots::generators::simple;

WaitForColorIntensityBlockGenerator::WaitForColorIntensityBlockGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "wait/colorIntensity.t", QList<Binding *>()
			<< Binding::createConverting("@@PORT@@", "Port", customizer.factory()->inputPortConverter())
			<< Binding::createConverting("@@INTENSITY@@", "Intensity", customizer.factory()->intPropertyConverter())
			<< Binding::createConverting("@@SIGN@@", "Sign", customizer.factory()->inequalitySignConverter())
			, parent)
{
}

//void WaitForColorIntensityBlockGenerator::addInitAndTerminateCode(NxtOSEKRobotGenerator *nxtGen
//		, QString const &port, qReal::Id const &elementId)
//{
//	QString const partInitCode = "ecrobot_init_nxtcolorsensor(" + port;
//	if (!ListSmartLine::isContainsPart(nxtGen->initCode(), partInitCode)) {
//		// i don't no, mb NXT_LIGHTSENSOR_NONE, NXT_COLORSENSOR_DEACTIVATE
//		QString const initCode = "ecrobot_init_nxtcolorsensor(" + port + "," + "NXT_COLORSENSOR);";
//		QString const terminateCode = "ecrobot_term_nxtcolorsensor(" + port + ");";
//		nxtGen->initCode().append(SmartLine_old(initCode, elementId));
//		nxtGen->terminateCode().append(SmartLine_old(terminateCode, elementId));
//	}
//}
