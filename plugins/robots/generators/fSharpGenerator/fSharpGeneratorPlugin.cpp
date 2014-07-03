#include "fSharpGeneratorPlugin.h"

#include <QtWidgets/QApplication>
#include <QtCore/QFileInfo>

#include <QtCore/QDebug>

//#include "robotCommunication/tcpRobotCommunicator.h"
#include "fSharpMasterGenerator.h"

using namespace fSharp;
using namespace qReal;

FSharpGeneratorPlugin::FSharpGeneratorPlugin()
	: mGenerateCodeAction(nullptr)
	, mUploadProgramAction(nullptr)
	, mRunProgramAction(nullptr)
	, mStopRobotAction(nullptr)
{
	mAppTranslator.load(":/FSharpGenerator_" + QLocale().name());
	QApplication::installTranslator(&mAppTranslator);
}

FSharpGeneratorPlugin::~FSharpGeneratorPlugin()
{
}

QString FSharpGeneratorPlugin::kitId() const
{
	return "trikKit";
}

QList<ActionInfo> FSharpGeneratorPlugin::actions()
{
	mGenerateCodeAction.setText(tr("Generate FSharp code"));
	mGenerateCodeAction.setIcon(QIcon(":/images/generateQtsCode.svg"));
	ActionInfo generateCodeActionInfo(&mGenerateCodeAction, "generators", "tools");
	connect(&mGenerateCodeAction, SIGNAL(triggered()), this, SLOT(generateCode()));

	return {generateCodeActionInfo};
}

QList<HotKeyActionInfo> FSharpGeneratorPlugin::hotKeyActions()
{
	mGenerateCodeAction.setShortcut(QKeySequence(Qt::CTRL + Qt::Key_G));
//	mUploadProgramAction.setShortcut(QKeySequence(Qt::CTRL + Qt::Key_U));
//	mRunProgramAction.setShortcut(QKeySequence(Qt::CTRL + Qt::Key_F5));
//	mStopRobotAction.setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_F5));

	HotKeyActionInfo generateCodeInfo("Generator.GenerateFSharp", tr("Generate fsharp Code"), &mGenerateCodeAction);
//	HotKeyActionInfo uploadProgramInfo("Generator.UploadFSharp", tr("Upload fsharp Program"), &mUploadProgramAction);
	//HotKeyActionInfo runProgramInfo("Generator.RunFSharp", tr("Run fsharp Program"), &mRunProgramAction);
	//HotKeyActionInfo stopRobotInfo("Generator.StopFSharp", tr("Stop fsharp Robot"), &mStopRobotAction);

	return { generateCodeInfo};
}

generatorBase::MasterGeneratorBase *FSharpGeneratorPlugin::masterGenerator()
{
	return new FSharpMasterGenerator(*mRepo
			, *mMainWindowInterface->errorReporter()
			, *mRobotModelManager
			, mMainWindowInterface->activeDiagram());
}

void FSharpGeneratorPlugin::regenerateExtraFiles(QFileInfo const &newFileInfo)
{
	Q_UNUSED(newFileInfo);
}

QString FSharpGeneratorPlugin::defaultFilePath(QString const &projectName) const
{
	return QString("trik/%1/%1.fs").arg(projectName);
}

QString FSharpGeneratorPlugin::extension() const
{
	return "fs";
}

QString FSharpGeneratorPlugin::extDescrition() const
{
	return tr("FSharp Source File");
}

QString FSharpGeneratorPlugin::generatorName() const
{
	return "FSharp";
}

bool FSharpGeneratorPlugin::uploadProgram()
{
	/*QFileInfo const fileInfo = generateCodeForProcessing();

	if (fileInfo != QFileInfo() && !fileInfo.absoluteFilePath().isEmpty()) {
		TcpRobotCommunicator communicator;
		bool const result = communicator.uploadProgram(fileInfo.absoluteFilePath());
		if (!result) {
			mMainWindowInterface->errorReporter()->addError(tr("No connection to robot"));
		}

		return result;
	} else {
		qDebug() << "Code generation failed, aborting";
		return false;
	}*/
	return 1;
}

void FSharpGeneratorPlugin::runProgram()
{
	/*if (uploadProgram()) {
		TcpRobotCommunicator communicator;
		QFileInfo const fileInfo = generateCodeForProcessing();
		communicator.runProgram(fileInfo.fileName());
	} else {
		qDebug() << "Program upload failed, aborting";
	}*/
}

void FSharpGeneratorPlugin::stopRobot()
{/*
	TcpRobotCommunicator communicator;
	if (!communicator.stopRobot()) {
		mMainWindowInterface->errorReporter()->addError(tr("No connection to robot"));
	}

	communicator.runDirectCommand(
			"brick.system(\"killall aplay\"); \n"
			"brick.system(\"killall vlc\"); \n"
			"brick.system(\"killall rover-cv\");"
			);
*/
}
