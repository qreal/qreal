#include "fSharpGeneratorPlugin.h"

#include <QtWidgets/QApplication>
#include <QtCore/QFileInfo>

#include <QtCore/QDebug>

#include <trikRobotCommunication/tcpRobotCommunicator.h>
#include "fSharpMasterGenerator.h"

using namespace qReal;
using namespace qReal::robots::generators;
using namespace fSharp;

FSharpGeneratorPlugin::FSharpGeneratorPlugin()
		: mGenerateCodeAction(NULL)
		, mUploadProgramAction(NULL)
		, mRunProgramAction(NULL)
		, mStopRobotAction(NULL)
{
	mAppTranslator.load(":/FSharpGenerator_" + QLocale::system().name());
	QApplication::installTranslator(&mAppTranslator);
}

FSharpGeneratorPlugin::~FSharpGeneratorPlugin()
{
}

QList<ActionInfo> FSharpGeneratorPlugin::actions()
{
	mGenerateCodeAction.setText(tr("Generate FSharp code"));
	ActionInfo generateCodeActionInfo(&mGenerateCodeAction, "generators", "tools");
	connect(&mGenerateCodeAction, SIGNAL(triggered()), this, SLOT(generateCode()));


	mUploadProgramAction.setText(tr("Upload program"));
	ActionInfo uploadProgramActionInfo(&mUploadProgramAction, "generators", "tools");
	connect(&mUploadProgramAction, SIGNAL(triggered()), this, SLOT(uploadProgram()));

	mRunProgramAction.setText(tr("Run program"));
	ActionInfo runProgramActionInfo(&mRunProgramAction, "generators", "tools");
	connect(&mRunProgramAction, SIGNAL(triggered()), this, SLOT(runProgram()));

	mStopRobotAction.setText(tr("Stop robot"));
	ActionInfo stopRobotActionInfo(&mStopRobotAction, "generators", "tools");
	connect(&mStopRobotAction, SIGNAL(triggered()), this, SLOT(stopRobot()));

	return QList<ActionInfo>()
			<< generateCodeActionInfo
			<< uploadProgramActionInfo
			<< runProgramActionInfo
			<< stopRobotActionInfo
			;
}

MasterGeneratorBase *FSharpGeneratorPlugin::masterGenerator()
{
	return new FSharpMasterGenerator(*mRepo
			, *mMainWindowInterface->errorReporter()
			, mMainWindowInterface->activeDiagram());
}

void FSharpGeneratorPlugin::regenerateExtraFiles(QFileInfo const &newFileInfo)
{
	Q_UNUSED(newFileInfo);
}

QString FSharpGeneratorPlugin::defaultFilePath(QString const &projectName) const
{
	return QString("trik/scripts/%1/%1.fsx").arg(projectName);
}

QString FSharpGeneratorPlugin::extension() const
{
	return "fsx";
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
	QFileInfo const fileInfo = generateCodeForProcessing();

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
	}
}

void FSharpGeneratorPlugin::runProgram()
{
	if (uploadProgram()) {
		TcpRobotCommunicator communicator;
		QFileInfo const fileInfo = generateCodeForProcessing();
		communicator.runDirectCommand("brick.system(\"mono \" " + fileInfo.fileName())+ " ); \n";
	} else {
		qDebug() << "Program upload failed, aborting";
	}
}

void FSharpGeneratorPlugin::stopRobot()
{
	TcpRobotCommunicator communicator;
	if (!communicator.stopRobot()) {
		mMainWindowInterface->errorReporter()->addError(tr("No connection to robot"));
	}

	communicator.runDirectCommand(
			"brick.system(\"killall aplay\"); \n"
			"brick.system(\"killall vlc\"); \n"
			"brick.system(\"killall rover-cv\");"
			);
}
