#include "ev3RbfGeneratorPlugin.h"

#include <QtWidgets/QApplication>
#include <QtCore/QDir>

#include "ev3RbfMasterGenerator.h"

using namespace ev3::rbf;

Ev3RbfGeneratorPlugin::Ev3RbfGeneratorPlugin()
	: mGenerateCodeAction(new QAction(nullptr))
{
}

QString Ev3RbfGeneratorPlugin::defaultFilePath(QString const &projectName) const
{
	return QString("ev3-rbf/%1/%1.rbf").arg(projectName);
}

QString Ev3RbfGeneratorPlugin::extension() const
{
	return QString("rbf");
}

QString Ev3RbfGeneratorPlugin::extensionDescription() const
{
	return tr("RBF Source File");
}

QString Ev3RbfGeneratorPlugin::generatorName() const
{
	return "ev3Rbf";
}

QList<qReal::ActionInfo> Ev3RbfGeneratorPlugin::actions()
{
	mGenerateCodeAction->setText(tr("Generate to Ev3 Robot Byte Code File"));
	mGenerateCodeAction->setIcon(QIcon(":/ev3/images/generateRbfCode.svg"));
	qReal::ActionInfo generateCodeActionInfo(mGenerateCodeAction, "generators", "tools");
	connect(mGenerateCodeAction, SIGNAL(triggered()), this, SLOT(generateCode()));

	return { generateCodeActionInfo };
}

QList<qReal::HotKeyActionInfo> Ev3RbfGeneratorPlugin::hotKeyActions()
{
	mGenerateCodeAction->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_G));
	qReal::HotKeyActionInfo generateActionInfo("Generator.GenerateEv3Rbf"
			, tr("Generate Ev3 Robot Byte Code File"), mGenerateCodeAction);

	return { generateActionInfo };
}

generatorBase::MasterGeneratorBase *Ev3RbfGeneratorPlugin::masterGenerator()
{
	return new Ev3RbfMasterGenerator(*mRepo
			, *mMainWindowInterface->errorReporter()
			, *mRobotModelManager
			, *mTextLanguage
			, mMainWindowInterface->activeDiagram()
			, generatorName());
}

void Ev3RbfGeneratorPlugin::regenerateExtraFiles(QFileInfo const &newFileInfo)
{
	Q_UNUSED(newFileInfo);
}
