#include "nxtRussianCGeneratorPlugin.h"

#include <QtWidgets/QApplication>
#include <QtCore/QDir>

#include "nxtRussianCMasterGenerator.h"

using namespace russianC;
using namespace generatorBase;

NxtRussianCGeneratorPlugin::NxtRussianCGeneratorPlugin()
	: mGenerateCodeAction(nullptr)
{
}

NxtRussianCGeneratorPlugin::~NxtRussianCGeneratorPlugin()
{
}

QString NxtRussianCGeneratorPlugin::kitId() const
{
	return "nxtKit";
}

QString NxtRussianCGeneratorPlugin::defaultFilePath(QString const &projectName) const
{
	return QString::fromUtf8("russianC/%1/%1.си").arg(projectName);
}

QString NxtRussianCGeneratorPlugin::extension() const
{
	return QString::fromUtf8("си");
}

QString NxtRussianCGeneratorPlugin::extensionDescription() const
{
	return tr("RussianC Source File");
}

QString NxtRussianCGeneratorPlugin::generatorName() const
{
	return "RussianC";
}

QList<qReal::ActionInfo> NxtRussianCGeneratorPlugin::actions()
{
	mGenerateCodeAction.setText(tr("Generate to Russian C"));
	mGenerateCodeAction.setIcon(QIcon(":/images/generateRussianCCode.svg"));
	qReal::ActionInfo generateCodeActionInfo(&mGenerateCodeAction, "generators", "tools");
	connect(&mGenerateCodeAction, SIGNAL(triggered()), this, SLOT(generateCode()));

	return { generateCodeActionInfo };
}

QList<qReal::HotKeyActionInfo> NxtRussianCGeneratorPlugin::hotKeyActions()
{
	mGenerateCodeAction.setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_G));

	qReal::HotKeyActionInfo generateActionInfo("Generator.GenerateNxtRussianC"
			, tr("Generate Russian C Code"), &mGenerateCodeAction);

	return { generateActionInfo };
}

MasterGeneratorBase *NxtRussianCGeneratorPlugin::masterGenerator()
{
	return new NxtRussianCMasterGenerator(*mRepo
			, *mMainWindowInterface->errorReporter()
			, *mRobotModelManager
			, *mTextLanguage
			, mMainWindowInterface->activeDiagram());
}


void NxtRussianCGeneratorPlugin::regenerateExtraFiles(QFileInfo const &newFileInfo)
{
	Q_UNUSED(newFileInfo);
}


void NxtRussianCGeneratorPlugin::changeActiveTab(QList<qReal::ActionInfo> const &info, bool trigger)
{
	for (qReal::ActionInfo const &actionInfo : info) {
		actionInfo.action()->setEnabled(trigger);
	}
}
