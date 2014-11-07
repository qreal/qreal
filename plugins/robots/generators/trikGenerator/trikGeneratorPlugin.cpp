#include "trikGeneratorPlugin.h"

#include <QtWidgets/QApplication>
#include <QtCore/QFileInfo>

#include <QtCore/QDebug>

#include "trikMasterGenerator.h"

using namespace trik;
using namespace qReal;

TrikGeneratorPlugin::TrikGeneratorPlugin()
	: mGenerateCodeAction(nullptr)
	, mUploadProgramAction(nullptr)
	, mRunProgramAction(nullptr)
	, mStopRobotAction(nullptr)
	, mCommunicator(nullptr)
{
}

TrikGeneratorPlugin::~TrikGeneratorPlugin()
{
	delete mCommunicator;
}

QString TrikGeneratorPlugin::kitId() const
{
	return "trikKit";
}

void TrikGeneratorPlugin::init(qReal::PluginConfigurator const &configurator
		, interpreterBase::robotModel::RobotModelManagerInterface const &robotModelManager
		, qrtext::LanguageToolboxInterface &textLanguage)
{
	RobotsGeneratorPluginBase::init(configurator, robotModelManager, textLanguage);
	mCommunicator = new utils::TcpRobotCommunicator("TrikTcpServer");
}

QList<ActionInfo> TrikGeneratorPlugin::actions()
{
	mGenerateCodeAction.setText(tr("Generate TRIK code"));
	mGenerateCodeAction.setIcon(QIcon(":/images/generateQtsCode.svg"));
	ActionInfo generateCodeActionInfo(&mGenerateCodeAction, "generators", "tools");
	connect(&mGenerateCodeAction, SIGNAL(triggered()), this, SLOT(generateCode()), Qt::UniqueConnection);

	mUploadProgramAction.setText(tr("Upload program"));
	mUploadProgramAction.setIcon(QIcon(":/images/uploadProgram.svg"));
	ActionInfo uploadProgramActionInfo(&mUploadProgramAction, "generators", "tools");
	connect(&mUploadProgramAction, SIGNAL(triggered()), this, SLOT(uploadProgram()), Qt::UniqueConnection);

	mRunProgramAction.setText(tr("Run program"));
	mRunProgramAction.setIcon(QIcon(":/images/uploadAndExecuteProgram.svg"));
	ActionInfo runProgramActionInfo(&mRunProgramAction, "generators", "tools");
	connect(&mRunProgramAction, SIGNAL(triggered()), this, SLOT(runProgram()), Qt::UniqueConnection);

	mStopRobotAction.setText(tr("Stop robot"));
	mStopRobotAction.setIcon(QIcon(":/images/stopRobot.svg"));
	ActionInfo stopRobotActionInfo(&mStopRobotAction, "generators", "tools");
	connect(&mStopRobotAction, SIGNAL(triggered()), this, SLOT(stopRobot()), Qt::UniqueConnection);

	return {generateCodeActionInfo, uploadProgramActionInfo, runProgramActionInfo, stopRobotActionInfo};
}

QList<HotKeyActionInfo> TrikGeneratorPlugin::hotKeyActions()
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

generatorBase::MasterGeneratorBase *TrikGeneratorPlugin::masterGenerator()
{
	return new TrikMasterGenerator(*mRepo
			, *mMainWindowInterface->errorReporter()
			, *mRobotModelManager
			, *mTextLanguage
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

QString TrikGeneratorPlugin::extensionDescription() const
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

void TrikGeneratorPlugin::runProgram()
{
	if (uploadProgram()) {
		QFileInfo const fileInfo = generateCodeForProcessing();
		mCommunicator->runProgram(fileInfo.fileName());
	} else {
		qDebug() << "Program upload failed, aborting";
	}
}

void TrikGeneratorPlugin::stopRobot()
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
