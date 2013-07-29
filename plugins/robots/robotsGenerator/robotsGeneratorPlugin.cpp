#include <QtWidgets/QApplication>
#include <QtWidgets/QMessageBox>

#include "robotsGeneratorPlugin.h"
#include "nxtOSEK/nxtOSEKRobotGenerator.h"
#include "../../../qrgui/mainwindow/qscintillaTextEdit.h"

using namespace qReal;
using namespace robots::generator;
using namespace gui;

RobotsGeneratorPlugin::RobotsGeneratorPlugin()
		: mGenerateCodeAction(NULL)
		, mFlashRobotAction(NULL)
		, mUploadProgramAction(NULL)
		, mNxtToolsPresent(false)
		, mCurrentCodeNumber(0)
{
	mAppTranslator.load(":/robotsGenerator_" + QLocale::system().name());
	QApplication::installTranslator(&mAppTranslator);
	checkNxtTools();
	initHotKeyActions();
}

RobotsGeneratorPlugin::~RobotsGeneratorPlugin()
{
	delete mFlashTool;
}

void RobotsGeneratorPlugin::init(PluginConfigurator const &configurator)
{
	mMainWindowInterface = &configurator.mainWindowInterpretersInterface();
	mRepoControlApi = &configurator.repoControlInterface();
	mProjectManager = &configurator.projectManager();
	mSystemEvents = &configurator.systemEvents();
	mTextManager = &configurator.textManager();

	mFlashTool = new NxtFlashTool(mMainWindowInterface->errorReporter());

	connect(mSystemEvents, SIGNAL(codePathChanged(qReal::Id, QFileInfo, QFileInfo))
			, this, SLOT(regenerateRobotSourceCode(qReal::Id, QFileInfo, QFileInfo)));
	connect(mSystemEvents, SIGNAL(newCodeAppeared(qReal::Id, QFileInfo)), this, SLOT(addNewCode(qReal::Id, QFileInfo)));
	connect(mSystemEvents, SIGNAL(diagramClosed(qReal::Id)), this, SLOT(removeDiagram(qReal::Id)));
	connect(mSystemEvents, SIGNAL(codeTabClosed(QFileInfo)), this, SLOT(removeCode(QFileInfo)));
}

QList<ActionInfo> RobotsGeneratorPlugin::actions()
{
	mGenerateCodeAction.setText(tr("Generate code"));
	mGenerateCodeAction.setIcon(QIcon(":/icons/robots_generate_nxt.png"));
	ActionInfo generateCodeActionInfo(&mGenerateCodeAction, "generators", "tools");
	connect(&mGenerateCodeAction, SIGNAL(triggered()), this, SLOT(generateRobotSourceCode()));

	mFlashRobotAction.setText(tr("Flash robot"));
	ActionInfo flashRobotActionInfo(&mFlashRobotAction, "generators", "tools");
	connect(&mFlashRobotAction, SIGNAL(triggered()), this, SLOT(flashRobot()));

	mUploadProgramAction.setText(tr("Upload program"));
	ActionInfo uploadProgramActionInfo(&mUploadProgramAction, "generators", "tools");
	connect(&mUploadProgramAction, SIGNAL(triggered()), this, SLOT(uploadProgram()));

	checkNxtTools();

	/*
	/// Set tabs, unused at the opening, enabled
	QList<ActionInfo> unusedAtTheOpeningTab;
	unusedAtTheOpeningTab << generateCodeActionInfo;
	changeActiveTab(unusedAtTheOpeningTab, false);
	*/
	return QList<ActionInfo>() << generateCodeActionInfo << flashRobotActionInfo
			<< uploadProgramActionInfo;
}

void RobotsGeneratorPlugin::initHotKeyActions()
{
	mGenerateCodeAction.setShortcut(QKeySequence(Qt::CTRL + Qt::Key_G));
	mUploadProgramAction.setShortcut(QKeySequence(Qt::CTRL + Qt::Key_U));

	HotKeyActionInfo generateActionInfo("Generator.Generate", tr("Generate code"), &mGenerateCodeAction);
	HotKeyActionInfo uploadActionInfo("Generator.Upload", tr("Upload program to robot"), &mUploadProgramAction);

	mHotKeyActionInfos << generateActionInfo << uploadActionInfo;
}

QList<HotKeyActionInfo> RobotsGeneratorPlugin::hotKeyActions()
{
	return mHotKeyActionInfos;
}

void RobotsGeneratorPlugin::changeActiveTab(QList<ActionInfo> const &info, bool const &trigger)
{
	foreach (ActionInfo const &actionInfo, info) {
			actionInfo.action()->setEnabled(trigger);
	}
}

bool RobotsGeneratorPlugin::generateRobotSourceCode()
{
	mProjectManager->save();

	robots::generator::NxtOSEKRobotGenerator gen(mMainWindowInterface->activeDiagram(),
			 *mRepoControlApi,
			 *mMainWindowInterface->errorReporter());
	mMainWindowInterface->errorReporter()->clearErrors();

	Id const &activeDiagram = mMainWindowInterface->activeDiagram();

	QString const projectName = "example" + QString::number(mCurrentCodeNumber);
	QFileInfo fileInfo = QFileInfo("nxt-tools/" + projectName + "/" + projectName + ".c");
	QList<QFileInfo> pathsList = mCodePath.values(activeDiagram);
	bool newCode = true;

	if (!pathsList.isEmpty()) {
		foreach(QFileInfo const &path, pathsList) {
			if (mTextManager->isDefaultPath(path.absoluteFilePath())
			&& (!mTextManager->isModifiedEver(path.absoluteFilePath()))) {
				fileInfo = path;
				newCode = false;
				break;
			}
		}
	}

	gen.generate(fileInfo);

	if (mMainWindowInterface->errorReporter()->wereErrors()) {
		return false;
	}

	if (newCode) {
		mCurrentCodeNumber++;
	}

	QFile file(fileInfo.absoluteFilePath());
	QTextStream *inStream = NULL;
	if (!file.isOpen() && file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		inStream = new QTextStream(&file);
	}

	if (inStream) {
		mMainWindowInterface->showInTextEditor(fileInfo, inStream->readAll());
	}
	return true;
}

void RobotsGeneratorPlugin::regenerateRobotSourceCode(qReal::Id const &diagram, QFileInfo const &oldFileInfo, QFileInfo const &newFileInfo)
{
	mTextManager->changeFilePath(oldFileInfo.absoluteFilePath(), newFileInfo.absoluteFilePath());

	mCodePath.remove(diagram, oldFileInfo);

	mCodePath.insert(diagram, newFileInfo);

	robots::generator::NxtOSEKRobotGenerator gen(diagram, *mRepoControlApi, *mMainWindowInterface->errorReporter());

	gen.generateOilAndMakeFile(newFileInfo);
}

void RobotsGeneratorPlugin::flashRobot()
{
	if (!mNxtToolsPresent) {
		mMainWindowInterface->errorReporter()->addError(tr("flash.sh not found. Make sure it is present in QReal installation directory"));
	} else {
		mFlashTool->flashRobot();
	}
}

void RobotsGeneratorPlugin::uploadProgram()
{
	if (!mNxtToolsPresent) {
		mMainWindowInterface->errorReporter()->addError(tr("upload.sh not found. Make sure it is present in QReal installation directory"));
	} else {
		QFileInfo fileInfo;
		Id const &activeDiagram = mMainWindowInterface->activeDiagram();

		if (activeDiagram != Id()) {
			if (generateRobotSourceCode()) {
				foreach(QFileInfo const &path, mCodePath.values(activeDiagram)) {
					if (mTextManager->isDefaultPath(path.absoluteFilePath())
					&& (!mTextManager->isModifiedEver(path.absoluteFilePath()))) {
						fileInfo = path;
						break;
					}
				}
			} else {
				return;
			}
		} else {
			QScintillaTextEdit *code = static_cast<QScintillaTextEdit *>(mMainWindowInterface->currentTab());
			fileInfo = QFileInfo(mTextManager->path(code));
		}

		mFlashTool->uploadProgram(fileInfo);
	}
}

void RobotsGeneratorPlugin::addNewCode(Id const &diagram, QFileInfo const &fileInfo)
{
	mCodePath.insert(diagram, fileInfo);
}

void RobotsGeneratorPlugin::removeDiagram(qReal::Id const &diagram)
{
	mCodePath.remove(diagram);
}

void RobotsGeneratorPlugin::removeCode(QFileInfo const &fileInfo)
{
	Id const &diagram = mCodePath.key(fileInfo);

	mCodePath.remove(diagram, fileInfo);
}

void RobotsGeneratorPlugin::checkNxtTools()
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
