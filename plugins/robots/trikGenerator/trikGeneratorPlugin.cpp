#include "trikGeneratorPlugin.h"

#include <QtWidgets/QApplication>
#include <QtCore/QFileInfo>

#include <QtCore/QDebug>

#include "robotCommunication/tcpRobotCommunicator.h"
#include "generator/trikRobotGenerator.h"

using namespace qReal;
using namespace robots::trikGenerator;

QString const scriptExtension = ".qts";

TrikGeneratorPlugin::TrikGeneratorPlugin()
		: mGenerateCodeAction(NULL)
		, mUploadProgramAction(NULL)
		, mRunProgramAction(NULL)
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
	mRepoControlApi = &configurator.repoControlInterface();
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

	return QList<ActionInfo>() << generateCodeActionInfo << uploadProgramActionInfo
			<< runProgramActionInfo;
}

bool TrikGeneratorPlugin::generateCode()
{
	mProjectManager->save();

	TrikRobotGenerator generator(mMainWindowInterface->activeDiagram()
			, *mRepoControlApi
			, *mMainWindowInterface->errorReporter()
			);

	mMainWindowInterface->errorReporter()->clearErrors();
	generator.generate(currentProgramName());

	if (mMainWindowInterface->errorReporter()->wereErrors()) {
		return false;
	}

	QTextStream *inStream = NULL;
	QFile inFile(currentProgramName());
	if (!inFile.isOpen() && inFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		inStream = new QTextStream(&inFile);
	}

	if (inStream) {
		mMainWindowInterface->showInTextEditor(currentProgramName(), inStream->readAll());
	}

	return true;
}

bool TrikGeneratorPlugin::uploadProgram()
{
	if (generateCode()) {
		TcpRobotCommunicator communicator;
		return communicator.uploadProgram(currentProgramName());
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

QString TrikGeneratorPlugin::currentProgramName() const
{
	QString const saveFileName = mRepoControlApi->workingFile();
	QFileInfo const fileInfo(saveFileName);
	return fileInfo.baseName() + scriptExtension;
}
