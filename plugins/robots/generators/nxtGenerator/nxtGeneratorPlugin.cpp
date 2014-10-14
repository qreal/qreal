#include "nxtGeneratorPlugin.h"

#include <QtWidgets/QApplication>
#include <QtCore/QDir>

#include <qrkernel/settingsManager.h>
#include <qrgui/mainwindow/qscintillaTextEdit.h>
#include <nxtOsekMasterGenerator.h>

using namespace nxtOsek;
using namespace qReal;
using namespace gui;

NxtGeneratorPlugin::NxtGeneratorPlugin()
	: mGenerateCodeAction(nullptr)
	, mFlashRobotAction(nullptr)
	, mUploadProgramAction(nullptr)
	, mNxtToolsPresent(false)
{
	checkNxtTools();
	initHotKeyActions();
}

NxtGeneratorPlugin::~NxtGeneratorPlugin()
{
	delete mFlashTool;
}

QString NxtGeneratorPlugin::kitId() const
{
	return "nxtKit";
}

QString NxtGeneratorPlugin::defaultFilePath(QString const &projectName) const
{
	return QString("nxt-tools/%1/%1.c").arg(projectName);
}

QString NxtGeneratorPlugin::extension() const
{
	return "c";
}

QString NxtGeneratorPlugin::extensionDescription() const
{
	return tr("Lego NXT Source File");
}

QString NxtGeneratorPlugin::generatorName() const
{
	return "nxtOsek";
}

bool NxtGeneratorPlugin::canGenerateTo(QString const &project)
{
	QString const cFilePath = QApplication::applicationDirPath() + "/" + defaultFilePath(project);
	QFileInfo const cFile(cFilePath);
	QFileInfo const makeFile(cFile.absolutePath() + "/makefile");
	if (!cFile.exists() || !makeFile.exists()) {
		return true;
	}

	// If c file has much later timestamp then it was edited by user - restrincting generation to this file.
	int const timestampMaxDifference = 100;
	return (cFile.lastModified().toMSecsSinceEpoch()
			- makeFile.lastModified().toMSecsSinceEpoch() < timestampMaxDifference);
}

void NxtGeneratorPlugin::init(PluginConfigurator const &configurator
		, interpreterBase::robotModel::RobotModelManagerInterface const &robotModelManager)
{
	RobotsGeneratorPluginBase::init(configurator, robotModelManager);

	mFlashTool = new NxtFlashTool(mMainWindowInterface->errorReporter());
	connect(mFlashTool, &NxtFlashTool::uploadingComplete, this, &NxtGeneratorPlugin::onUploadingComplete);
}

QList<ActionInfo> NxtGeneratorPlugin::actions()
{
	checkNxtTools();

	mGenerateCodeAction.setText(tr("Generate code"));
	mGenerateCodeAction.setIcon(QIcon(":/nxt/images/generateOsekCode.svg"));
	ActionInfo generateCodeActionInfo(&mGenerateCodeAction, "generators", "tools");
	connect(&mGenerateCodeAction, SIGNAL(triggered()), this, SLOT(generateCode()));

	mFlashRobotAction.setText(tr("Flash robot"));
	mFlashRobotAction.setIcon(QIcon(":/nxt/images/flashRobot.svg"));
	ActionInfo flashRobotActionInfo(&mFlashRobotAction, "generators", "tools");
	connect(&mFlashRobotAction, SIGNAL(triggered()), this, SLOT(flashRobot()));

	mUploadProgramAction.setText(tr("Upload program"));
	mUploadProgramAction.setIcon(QIcon(":/nxt/images/uploadProgram.svg"));
	ActionInfo uploadProgramActionInfo(&mUploadProgramAction, "generators", "tools");
	connect(&mUploadProgramAction, SIGNAL(triggered()), this, SLOT(uploadProgram()));

	return mNxtToolsPresent
			? QList<ActionInfo>() << generateCodeActionInfo << flashRobotActionInfo << uploadProgramActionInfo
			: QList<ActionInfo>() << generateCodeActionInfo;
}

void NxtGeneratorPlugin::initHotKeyActions()
{
	mGenerateCodeAction.setShortcut(QKeySequence(Qt::CTRL + Qt::Key_G));
	mUploadProgramAction.setShortcut(QKeySequence(Qt::CTRL + Qt::Key_U));

	HotKeyActionInfo generateActionInfo("Generator.GenerateNxt", tr("Generate NXT OSEK code"), &mGenerateCodeAction);
	HotKeyActionInfo uploadActionInfo("Generator.UploadNxt", tr("Upload program to NXT device"), &mUploadProgramAction);

	mHotKeyActionInfos << generateActionInfo << uploadActionInfo;
}

void NxtGeneratorPlugin::onUploadingComplete(bool success)
{
	if (!success) {
		return;
	}

	NxtFlashTool::RunPolicy const runPolicy = static_cast<NxtFlashTool::RunPolicy>(
			SettingsManager::value("nxtFlashToolRunPolicy").toInt());

	switch (runPolicy) {
	case NxtFlashTool::Ask:
		if (mFlashTool->askToRun(mMainWindowInterface->windowWidget())) {
			mFlashTool->runLastProgram();
		}
		break;
	case NxtFlashTool::AlwaysRun:
		mFlashTool->runLastProgram();
		break;
	default:
		break;
	}
}

QList<HotKeyActionInfo> NxtGeneratorPlugin::hotKeyActions()
{
	return mHotKeyActionInfos;
}

generatorBase::MasterGeneratorBase *NxtGeneratorPlugin::masterGenerator()
{
	return new nxtOsek::NxtOsekMasterGenerator(*mRepo
			, *mMainWindowInterface->errorReporter()
			, *mRobotModelManager
			, mMainWindowInterface->activeDiagram());
}

void NxtGeneratorPlugin::regenerateExtraFiles(QFileInfo const &newFileInfo)
{
	nxtOsek::NxtOsekMasterGenerator * const generator = new nxtOsek::NxtOsekMasterGenerator(*mRepo
		, *mMainWindowInterface->errorReporter()
		, *mRobotModelManager
		, mMainWindowInterface->activeDiagram());
	generator->initialize();
	generator->setProjectDir(newFileInfo);
	generator->generateOilAndMakeFiles();
}

void NxtGeneratorPlugin::changeActiveTab(QList<ActionInfo> const &info, bool const &trigger)
{
	foreach (ActionInfo const &actionInfo, info) {
		actionInfo.action()->setEnabled(trigger);
	}
}

void NxtGeneratorPlugin::flashRobot()
{
	if (!mNxtToolsPresent) {
		mMainWindowInterface->errorReporter()->addError(tr("flash.sh not found."\
				" Make sure it is present in QReal installation directory"));
	} else {
		mFlashTool->flashRobot();
	}
}

void NxtGeneratorPlugin::uploadProgram()
{
	if (!mNxtToolsPresent) {
		mMainWindowInterface->errorReporter()->addError(tr("upload.sh not found. Make sure it is present in QReal installation directory"));
	} else {
		QFileInfo const fileInfo = generateCodeForProcessing();

		if (fileInfo != QFileInfo()) {
			mFlashTool->uploadProgram(fileInfo);
		}
	}
}

void NxtGeneratorPlugin::checkNxtTools()
{
	QDir dir(qApp->applicationDirPath());
	if (!QDir().exists(dir.absolutePath() + "/nxt-tools")) {
		mNxtToolsPresent = false;
	} else {
		dir.cd(dir.absolutePath() + "/nxt-tools");

		QDir gnuarm(dir.absolutePath() + "/gnuarm");
		QDir nexttool(dir.absolutePath() + "/nexttool");
		QDir nxtOSEK(dir.absolutePath() + "/nxtOSEK");

#ifdef Q_OS_WIN
		QFile flash(dir.absolutePath() + "/flash.bat");
		QFile upload1(dir.absolutePath() + "/upload.bat");
		QFile upload2(dir.absolutePath() + "/upload.sh");

		mNxtToolsPresent = gnuarm.exists() && nexttool.exists() && nxtOSEK.exists() && flash.exists() && upload1.exists() && upload2.exists();
#else
		QDir libnxt(dir.absolutePath() + "/libnxt");
		QFile flash(dir.absolutePath() + "/flash.sh");
		QFile upload(dir.absolutePath() + "/upload.sh");

		mNxtToolsPresent = gnuarm.exists() && libnxt.exists() && nexttool.exists() && nxtOSEK.exists() && flash.exists() && upload.exists();
#endif
	}

	mUploadProgramAction.setVisible(mNxtToolsPresent);
	mFlashRobotAction.setVisible(mNxtToolsPresent);
}
