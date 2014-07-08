#include "russianCGeneratorPlugin.h"

#include <QtWidgets/QApplication>
#include <QtCore/QDir>

#include "russianCMasterGenerator.h"

using namespace russianC;
using namespace generatorBase;

RussianCGeneratorPlugin::RussianCGeneratorPlugin()
	: mGenerateCodeAction(nullptr)
{
	mAppTranslator.load(":/russianCGenerator_" + QLocale().name());
	QApplication::installTranslator(&mAppTranslator);
}

RussianCGeneratorPlugin::~RussianCGeneratorPlugin()
{
}

QString RussianCGeneratorPlugin::kitId() const
{
	return "nxtKit";
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

QList<ActionInfo> RussianCGeneratorPlugin::actions()
{
	mGenerateCodeAction.setText(tr("Generate to Russian C"));
	mGenerateCodeAction.setIcon(QIcon(":/images/generateRussianCCode.svg"));
	ActionInfo generateCodeActionInfo(&mGenerateCodeAction, "generators", "tools");
	mGenerateCodeAction.setObjectName("generateRussianC");
	connect(&mGenerateCodeAction, SIGNAL(triggered()), this, SLOT(generateCode()));

	return { generateCodeActionInfo };
}

QList<HotKeyActionInfo> RussianCGeneratorPlugin::hotKeyActions()
{
	mGenerateCodeAction.setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_G));

	HotKeyActionInfo generateActionInfo("Generator.GenerateNxtRussianC"
			, tr("Generate Russian C Code"), &mGenerateCodeAction);

	return { generateActionInfo };
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
