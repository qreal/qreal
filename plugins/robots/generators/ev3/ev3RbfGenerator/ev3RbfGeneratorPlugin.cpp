#include "ev3RbfGeneratorPlugin.h"

#include <QtWidgets/QApplication>
#include <QtCore/QDir>

#include "ev3RbfMasterGenerator.h"

using namespace ev3::rbf;

Ev3RbfGeneratorPlugin::Ev3RbfGeneratorPlugin()
	: mGenerateCodeAction(new QAction(nullptr))
	, mUploadProgramAction(new QAction(nullptr))
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

bool Ev3RbfGeneratorPlugin::uploadProgram()
{
	if (!javaInstalled()) {
		mMainWindowInterface->errorReporter()->addError(tr("Java JRE not found"));
		return false;
	}
	return true;
}

bool Ev3RbfGeneratorPlugin::javaInstalled()
{
	QProcess myProcess;
	myProcess.start("java");
	myProcess.waitForFinished();
	return !myProcess.readAllStandardError().isEmpty();
}

QList<qReal::ActionInfo> Ev3RbfGeneratorPlugin::actions()
{
	mGenerateCodeAction->setText(tr("Generate to Ev3 Robot Byte Code File"));
	mGenerateCodeAction->setIcon(QIcon(":/ev3/images/generateRbfCode.svg"));
	qReal::ActionInfo generateCodeActionInfo(mGenerateCodeAction, "generators", "tools");
	connect(mGenerateCodeAction, SIGNAL(triggered()), this, SLOT(generateCode()));

	mUploadProgramAction->setText(tr("Upload program"));
	mUploadProgramAction->setIcon(QIcon(":/ev3/images/uploadProgram.svg"));
	qReal::ActionInfo uploadProgramActionInfo(mUploadProgramAction, "generators", "tools");
	connect(mUploadProgramAction, SIGNAL(triggered()), this, SLOT(uploadProgram()));

	return { generateCodeActionInfo, uploadProgramActionInfo };
}

QList<qReal::HotKeyActionInfo> Ev3RbfGeneratorPlugin::hotKeyActions()
{
	mGenerateCodeAction->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_G));
	mUploadProgramAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_U));
	qReal::HotKeyActionInfo generateActionInfo("Generator.GenerateEv3Rbf"
			, tr("Generate Ev3 Robot Byte Code File"), mGenerateCodeAction);
	qReal::HotKeyActionInfo uploadProgramInfo("Generator.UploadEv3", tr("Upload EV3 Program"), mUploadProgramAction);

	return { generateActionInfo, uploadProgramInfo };
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
