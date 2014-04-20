#include "trikGeneratorPlugin.h"

#include <QtWidgets/QApplication>
#include <QtCore/QFileInfo>

#include <QtCore/QDebug>

#include "robotCommunication/tcpRobotCommunicator.h"
#include "trikMasterGenerator.h"

using namespace trik;
using namespace qReal;

TrikGeneratorPlugin::TrikGeneratorPlugin()
		: mGenerateCodeAction(NULL)
		, mUploadProgramAction(NULL)
		, mRunProgramAction(NULL)
		, mStopRobotAction(NULL)
{
	mAppTranslator.load(":/trikGenerator_" + QLocale::system().name());
	QApplication::installTranslator(&mAppTranslator);
}

TrikGeneratorPlugin::~TrikGeneratorPlugin()
{
}

QList<ActionInfo> TrikGeneratorPlugin::actions()
{
	mGenerateCodeAction.setText(tr("Generate TRIK code"));
	mGenerateCodeAction.setIcon(QIcon(":/images/generateQtsCode.svg"));
	ActionInfo generateCodeActionInfo(&mGenerateCodeAction, "generators", "tools");
	connect(&mGenerateCodeAction, SIGNAL(triggered()), this, SLOT(generateCode()));

	mUploadProgramAction.setText(tr("Upload program"));
	mUploadProgramAction.setIcon(QIcon(":/images/uploadProgram.svg"));
	ActionInfo uploadProgramActionInfo(&mUploadProgramAction, "generators", "tools");
	connect(&mUploadProgramAction, SIGNAL(triggered()), this, SLOT(uploadProgram()));

	mRunProgramAction.setText(tr("Run program"));
	mRunProgramAction.setIcon(QIcon(":/images/uploadAndExecuteProgram.svg"));
	ActionInfo runProgramActionInfo(&mRunProgramAction, "generators", "tools");
	connect(&mRunProgramAction, SIGNAL(triggered()), this, SLOT(runProgram()));

	mStopRobotAction.setText(tr("Stop robot"));
	mStopRobotAction.setIcon(QIcon(":/images/stopRobot.svg"));
	ActionInfo stopRobotActionInfo(&mStopRobotAction, "generators", "tools");
	connect(&mStopRobotAction, SIGNAL(triggered()), this, SLOT(stopRobot()));

	return {generateCodeActionInfo, uploadProgramActionInfo, runProgramActionInfo, stopRobotActionInfo};
}

generatorBase::MasterGeneratorBase *TrikGeneratorPlugin::masterGenerator()
{
	return new TrikMasterGenerator(*mRepo
			, *mMainWindowInterface->errorReporter()
			, mMainWindowInterface->activeDiagram());
}

void TrikGeneratorPlugin::regenerateExtraFiles(QFileInfo const &newFileInfo)
{
	Q_UNUSED(newFileInfo);
}

QString TrikGeneratorPlugin::defaultFilePath(QString const &projectName) const
{
	return QString("trik/%1/%1.qts").arg(projectName);
}

QString TrikGeneratorPlugin::extension() const
{
	return "qts";
}

QString TrikGeneratorPlugin::extDescrition() const
{
	return tr("TRIK Source File");
}

QString TrikGeneratorPlugin::generatorName() const
{
	return "Trik";
}

bool TrikGeneratorPlugin::uploadProgram()
{
	QFileInfo const fileInfo = generateCodeForProcessing();

	if (fileInfo != QFileInfo()) {
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

void TrikGeneratorPlugin::runProgram()
{
	if (uploadProgram()) {
		TcpRobotCommunicator communicator;
		QFileInfo const fileInfo = generateCodeForProcessing();
		communicator.runProgram(fileInfo.fileName());
	} else {
		qDebug() << "Program upload failed, aborting";
	}
}

void TrikGeneratorPlugin::stopRobot()
{
	TcpRobotCommunicator communicator;
	if (!communicator.stopRobot()) {
		mMainWindowInterface->errorReporter()->addError(tr("No connection to robot"));
	}
}
