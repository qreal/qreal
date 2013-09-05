#include "waitForColorBlockGenerator.h"
#include "../generatorCustomizer.h"

using namespace qReal::robots::generators::simple;

WaitForColorBlockGenerator::WaitForColorBlockGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "wait/color.t", QList<Binding *>()
			<< Binding::createConverting("@@PORT@@", "Port", customizer.factory()->inputPortConverter())
			<< Binding::createConverting("@@COLOR@@", "Color", customizer.factory()->colorConverter())
			, parent)
{
}

//void WaitForColorBlockGenerator::addInitAndTerminateCode(NxtOSEKRobotGenerator *nxtGen,
//		QString const &port, QString const &colorNxtType, qReal::Id const &elementId)
//{
//	QString const partInitCode = "ecrobot_init_nxtcolorsensor(" + port;
//	QString const initCode = "ecrobot_init_nxtcolorsensor(" + port + "," + colorNxtType + ");";

//	if (!ListSmartLine::isContainsPart(nxtGen->initCode(), partInitCode)) {
//		QString const terminateCode = "ecrobot_init_nxtcolorsensor(" + port + "," + colorNxtType + ");";
//		nxtGen->initCode().append(SmartLine_old(initCode, elementId));
//		nxtGen->terminateCode().append(SmartLine_old(terminateCode, elementId));
//	}
//}
