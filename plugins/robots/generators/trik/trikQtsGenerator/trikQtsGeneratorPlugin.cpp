#include "trikQtsGeneratorPlugin.h"

#include <QtWidgets/QApplication>
#include <QtCore/QFileInfo>
#include <QtCore/QDebug>

#include <trikGeneratorBase/trikGeneratorPluginBase.h>
#include <utils/tcpRobotCommunicator.h>

#include "trikQtsMasterGenerator.h"

using namespace trik::qts;
using namespace qReal;

TrikQtsGeneratorPlugin::TrikQtsGeneratorPlugin()
	: mGenerateCodeAction(nullptr)
	, mUploadProgramAction(nullptr)
	, mRunProgramAction(nullptr)
	, mStopRobotAction(nullptr)
	, mCommunicator(nullptr)
{
}

TrikQtsGeneratorPlugin::~TrikQtsGeneratorPlugin()
{
	delete mCommunicator;
}

void TrikQtsGeneratorPlugin::init(qReal::PluginConfigurator const &configurator
		, interpreterBase::robotModel::RobotModelManagerInterface const &robotModelManager
		, qrtext::LanguageToolboxInterface &textLanguage)
{
	RobotsGeneratorPluginBase::init(configurator, robotModelManager, textLanguage);
	mCommunicator = new utils::TcpRobotCommunicator("TrikTcpServer");
	mCommunicator->setErrorReporter(configurator.mainWindowInterpretersInterface().errorReporter());
}

QList<ActionInfo> TrikQtsGeneratorPlugin::actions()
{
	QAction *separator = new QAction(this);
	separator->setSeparator(true);
	qReal::ActionInfo separatorInfo(separator, "generators", "tools");

	mGenerateCodeAction.setText(tr("Generate TRIK code"));
	mGenerateCodeAction.setIcon(QIcon(":/images/generateQtsCode.svg"));
	ActionInfo generateCodeActionInfo(&mGenerateCodeAction, "generators", "tools");
	mGenerateCodeAction.setObjectName("generateTRIKCode");
	connect(&mGenerateCodeAction, SIGNAL(triggered()), this, SLOT(generateCode()), Qt::UniqueConnection);

	mUploadProgramAction.setText(tr("Upload program"));
	mUploadProgramAction.setIcon(QIcon(":/images/uploadProgram.svg"));
	ActionInfo uploadProgramActionInfo(&mUploadProgramAction, "generators", "tools");
	mUploadProgramAction.setObjectName("uploadProgram");
	connect(&mUploadProgramAction, SIGNAL(triggered()), this, SLOT(uploadProgram()), Qt::UniqueConnection);

	mRunProgramAction.setText(tr("Run program"));
	mRunProgramAction.setIcon(QIcon(":/images/uploadAndExecuteProgram.svg"));
	ActionInfo runProgramActionInfo(&mRunProgramAction, "generators", "tools");
	mRunProgramAction.setObjectName("runProgram");
	connect(&mRunProgramAction, SIGNAL(triggered()), this, SLOT(runProgram()), Qt::UniqueConnection);

	mStopRobotAction.setText(tr("Stop robot"));
	mStopRobotAction.setIcon(QIcon(":/images/stopRobot.svg"));
	ActionInfo stopRobotActionInfo(&mStopRobotAction, "generators", "tools");
	mStopRobotAction.setObjectName("stopRobot");
	connect(&mStopRobotAction, SIGNAL(triggered()), this, SLOT(stopRobot()), Qt::UniqueConnection);

	return {generateCodeActionInfo, uploadProgramActionInfo, runProgramActionInfo, stopRobotActionInfo, separatorInfo};
}

QList<HotKeyActionInfo> TrikQtsGeneratorPlugin::hotKeyActions()
{
	mGenerateCodeAction.setShortcut(QKeySequence(Qt::CTRL + Qt::Key_G));
	mUploadProgramAction.setShortcut(QKeySequence(Qt::CTRL + Qt::Key_U));
	mRunProgramAction.setShortcut(QKeySequence(Qt::CTRL + Qt::Key_F5));
	mStopRobotAction.setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_F5));

	HotKeyActionInfo generateCodeInfo("Generator.GenerateTrik", tr("Generate TRIK Code"), &mGenerateCodeAction);
	HotKeyActionInfo uploadProgramInfo("Generator.UploadTrik", tr("Upload TRIK Program"), &mUploadProgramAction);
	HotKeyActionInfo runProgramInfo("Generator.RunTrik", tr("Run TRIK Program"), &mRunProgramAction);
	HotKeyActionInfo stopRobotInfo("Generator.StopTrik", tr("Stop TRIK Robot"), &mStopRobotAction);

	return { generateCodeInfo, uploadProgramInfo, runProgramInfo, stopRobotInfo };
}

generatorBase::MasterGeneratorBase *TrikQtsGeneratorPlugin::masterGenerator()
{
	return new TrikQtsMasterGenerator(*mRepo
			, *mMainWindowInterface->errorReporter()
			, *mRobotModelManager
			, *mTextLanguage
			, mMainWindowInterface->activeDiagram()
			, generatorName());
}

QString TrikQtsGeneratorPlugin::defaultFilePath(QString const &projectName) const
{
	return QString("trik/%1/%1.qts").arg(projectName);
}

QString TrikQtsGeneratorPlugin::extension() const
{
	return "qts";
}

QString TrikQtsGeneratorPlugin::extensionDescription() const
{
	return tr("QtScript Source File");
}

QString TrikQtsGeneratorPlugin::generatorName() const
{
	return "trikQts";
}

bool TrikQtsGeneratorPlugin::uploadProgram()
{
	QFileInfo const fileInfo = generateCodeForProcessing();

	if (fileInfo != QFileInfo() && !fileInfo.absoluteFilePath().isEmpty()) {
		bool const result = mCommunicator->uploadProgram(fileInfo.absoluteFilePath());
		if (!result) {
			mMainWindowInterface->errorReporter()->addError(tr("No connection to robot"));
		}

		return result;
	} else {
		qDebug() << "Code generation failed, aborting";
		return false;
	}
}

void TrikQtsGeneratorPlugin::runProgram()
{
	if (uploadProgram()) {
		QFileInfo const fileInfo = generateCodeForProcessing();
		mCommunicator->runProgram(fileInfo.fileName());
	} else {
		qDebug() << "Program upload failed, aborting";
	}
}

void TrikQtsGeneratorPlugin::stopRobot()
{
	if (!mCommunicator->stopRobot()) {
		mMainWindowInterface->errorReporter()->addError(tr("No connection to robot"));
	}

	mCommunicator->runDirectCommand(
			"brick.system(\"killall aplay\"); \n"
			"brick.system(\"killall vlc\"); \n"
			"brick.system(\"killall rover-cv\");"
			, true
			);
}
