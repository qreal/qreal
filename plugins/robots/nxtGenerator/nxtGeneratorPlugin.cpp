#include "nxtGeneratorPlugin.h"

#include <QtWidgets/QApplication>
#include <QtCore/QDir>

#include <nxtOsekMasterGenerator.h>

using namespace qReal;
using namespace qReal::robots::generators;

NxtGeneratorPlugin::NxtGeneratorPlugin()
	: mGenerateCodeAction(NULL)
	, mFlashRobotAction(NULL)
	, mUploadProgramAction(NULL)
	, mNxtToolsPresent(false)
{
	mAppTranslator.load(":/nxtGenerator_" + QLocale::system().name());
	QApplication::installTranslator(&mAppTranslator);
	checkNxtTools();
	initHotKeyActions();
}

NxtGeneratorPlugin::~NxtGeneratorPlugin()
{
	delete mFlashTool;
}

void NxtGeneratorPlugin::init(PluginConfigurator const &configurator)
{
	RobotsGeneratorPluginBase::init(configurator);
	mFlashTool = new NxtFlashTool(mMainWindowInterface->errorReporter());
}

QList<ActionInfo> NxtGeneratorPlugin::actions()
{
	mGenerateCodeAction.setText(tr("Generate code"));
	mGenerateCodeAction.setIcon(QIcon(":/icons/robots_generate_nxt.png"));
	ActionInfo generateCodeActionInfo(&mGenerateCodeAction, "generators", "tools");
	connect(&mGenerateCodeAction, SIGNAL(triggered()), this, SLOT(generateCode()));

	mFlashRobotAction.setText(tr("Flash robot"));
	ActionInfo flashRobotActionInfo(&mFlashRobotAction, "generators", "tools");
	connect(&mFlashRobotAction, SIGNAL(triggered()), this, SLOT(flashRobot()));

	mUploadProgramAction.setText(tr("Upload program"));
	ActionInfo uploadProgramActionInfo(&mUploadProgramAction, "generators", "tools");
	connect(&mUploadProgramAction, SIGNAL(triggered()), this, SLOT(uploadProgram()));

	checkNxtTools();

	return QList<ActionInfo>() << generateCodeActionInfo << flashRobotActionInfo
			<< uploadProgramActionInfo;
}

void NxtGeneratorPlugin::initHotKeyActions()
{
	mGenerateCodeAction.setShortcut(QKeySequence(Qt::CTRL + Qt::Key_G));
	mUploadProgramAction.setShortcut(QKeySequence(Qt::CTRL + Qt::Key_U));

	HotKeyActionInfo generateActionInfo("Generator.Generate", tr("Generate code"), &mGenerateCodeAction);
	HotKeyActionInfo uploadActionInfo("Generator.Upload", tr("Upload program to robot"), &mUploadProgramAction);

	mHotKeyActionInfos << generateActionInfo << uploadActionInfo;
}

QList<HotKeyActionInfo> NxtGeneratorPlugin::hotKeyActions()
{
	return mHotKeyActionInfos;
}

MasterGeneratorBase *NxtGeneratorPlugin::masterGenerator()
{
	return new nxtOsek::NxtOsekMasterGenerator(*mRepo
			, *mMainWindowInterface->errorReporter()
			, mMainWindowInterface->activeDiagram());
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
		mMainWindowInterface->errorReporter()->addError(tr("upload.sh not found."\
				" Make sure it is present in QReal installation directory"));
	} else {
		if (generateCode()) {
			mFlashTool->uploadProgram();
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
