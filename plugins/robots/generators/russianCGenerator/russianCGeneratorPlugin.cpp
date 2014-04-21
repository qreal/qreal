#include "russianCGeneratorPlugin.h"

#include <QtWidgets/QApplication>
#include <QtCore/QDir>

#include "russianCMasterGenerator.h"

using namespace russianC;
using namespace generatorBase;

RussianCGeneratorPlugin::RussianCGeneratorPlugin()
	: mGenerateCodeAction(nullptr)
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

QString RussianCGeneratorPlugin::kitId() const
{
	return "nxtKit";
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
	return new RussianCMasterGenerator(*mRepo
			, *mMainWindowInterface->errorReporter()
			, *mRobotModelManager
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
