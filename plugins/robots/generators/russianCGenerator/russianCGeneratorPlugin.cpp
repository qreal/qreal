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

QString RussianCGeneratorPlugin::defaultFilePath(QString const &projectName) const
{
	return QString("russianC/%1/%1.c").arg(projectName);
}

QString RussianCGeneratorPlugin::extension() const
{
	return "c";
}

QString RussianCGeneratorPlugin::extDescrition() const
{
	return tr("RussianC Source File");
}

QString RussianCGeneratorPlugin::generatorName() const
{
	return "RussianC";
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
	mGenerateCodeAction.setIcon(QIcon(":/images/generateRussianCCode.svg"));
	ActionInfo generateCodeActionInfo(&mGenerateCodeAction, "generators", "tools");
	connect(&mGenerateCodeAction, SIGNAL(triggered()), this, SLOT(generateCode()));

	return { generateCodeActionInfo };
}

MasterGeneratorBase *RussianCGeneratorPlugin::masterGenerator()
{
	return new russianC::RussianCMasterGenerator(*mRepo
			, *mMainWindowInterface->errorReporter()
			, mMainWindowInterface->activeDiagram());
}


void RussianCGeneratorPlugin::regenerateExtraFiles(QFileInfo const &newFileInfo)
{
	Q_UNUSED(newFileInfo);
}


void RussianCGeneratorPlugin::changeActiveTab(QList<ActionInfo> const &info, bool trigger)
{
	foreach (ActionInfo const &actionInfo, info) {
		actionInfo.action()->setEnabled(trigger);
	}
}
