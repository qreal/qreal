#include "russianCGeneratorPlugin.h"

#include <QtWidgets/QApplication>
#include <QtCore/QDir>

#include "russianCMasterGenerator.h"

using namespace qReal;
using namespace qReal::robots::generators;

RussianCGeneratorPlugin::RussianCGeneratorPlugin()
	: mGenerateCodeAction(NULL)
{
	mAppTranslator.load(":/russianCGenerator_" + QLocale::system().name());
	QApplication::installTranslator(&mAppTranslator);
}

RussianCGeneratorPlugin::~RussianCGeneratorPlugin()
{
}

void RussianCGeneratorPlugin::init(PluginConfigurator const &configurator)
{
	RobotsGeneratorPluginBase::init(configurator);
}

QList<ActionInfo> RussianCGeneratorPlugin::actions()
{
	mGenerateCodeAction.setText(tr("Generate to Russian C"));
	ActionInfo generateCodeActionInfo(&mGenerateCodeAction, "generators", "tools");
	connect(&mGenerateCodeAction, SIGNAL(triggered()), this, SLOT(generateCode()));

	return QList<ActionInfo>() << generateCodeActionInfo;
}

MasterGeneratorBase *RussianCGeneratorPlugin::masterGenerator()
{
	return new russianC::RussianCMasterGenerator(*mRepo
			, *mMainWindowInterface->errorReporter()
			, mMainWindowInterface->activeDiagram());
}

void RussianCGeneratorPlugin::changeActiveTab(QList<ActionInfo> const &info, bool trigger)
{
	foreach (ActionInfo const &actionInfo, info) {
		actionInfo.action()->setEnabled(trigger);
	}
}
