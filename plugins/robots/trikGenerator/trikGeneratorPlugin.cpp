#include "trikGeneratorPlugin.h"

#include <QtWidgets/QApplication>
#include <QtCore/QFileInfo>

#include <QtCore/QDebug>

#include <qrutils/inFile.h>
#include "robotCommunication/tcpRobotCommunicator.h"
#include "trikMasterGenerator.h"

using namespace qReal;
using namespace qReal::robots::generators::trik;

QString const scriptExtension = ".qts";

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

void TrikGeneratorPlugin::init(PluginConfigurator const &configurator)
{
	mMainWindowInterface = &configurator.mainWindowInterpretersInterface();
	mRepo = dynamic_cast<qrRepo::RepoApi const *>(&configurator.logicalModelApi().logicalRepoApi());
	mProjectManager = &configurator.projectManager();
}

QList<ActionInfo> TrikGeneratorPlugin::actions()
{
	mGenerateCodeAction.setText(tr("Generate TRIK code"));
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

	return QList<ActionInfo>() << generateCodeActionInfo << uploadProgramActionInfo
			<< runProgramActionInfo << stopRobotActionInfo;
}

bool TrikGeneratorPlugin::generateCode()
{
	mProjectManager->save();
	mMainWindowInterface->errorReporter()->clearErrors();

	TrikMasterGenerator generator(*mRepo
			, *mMainWindowInterface->errorReporter()
			, mMainWindowInterface->activeDiagram());
	generator.initialize();

	QString const generatedSrcPath = generator.generate();
	if (mMainWindowInterface->errorReporter()->wereErrors()) {
		return false;
	}

	QString const generatedCode = utils::InFile::readAll(generatedSrcPath);
	if (!generatedCode.isEmpty()) {
		mMainWindowInterface->showInTextEditor(tr("Generated code"), generatedCode);
	}

	return true;
}

bool TrikGeneratorPlugin::uploadProgram()
{
	if (generateCode()) {
		TcpRobotCommunicator communicator;
		bool const result = communicator.uploadProgram(currentProgramName());
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
		communicator.runProgram(currentProgramName());
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

QString TrikGeneratorPlugin::currentProgramName() const
{
	QString const saveFileName = mRepo->workingFile();
	QFileInfo const fileInfo(saveFileName);
	return fileInfo.baseName() + scriptExtension;
}
