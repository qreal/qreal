#include <QtGui/QApplication>

#include "robotsGeneratorPlugin.h"
#include "nxtOSEK/nxtOSEKRobotGenerator.h"

Q_EXPORT_PLUGIN2(robotsGeneratorPlugin, robots::generator::RobotsGeneratorPlugin)

using namespace qReal;
using namespace robots::generator;

RobotsGeneratorPlugin::RobotsGeneratorPlugin()
        : mGenerateCodeAction(NULL)
        , mFlashRobotAction(NULL)
        , mUploadProgramAction(NULL)
        , mNxtToolsPresent(false)
{
    mAppTranslator.load(":/robotsGenerator_" + QLocale::system().name());
    QApplication::installTranslator(&mAppTranslator);
    checkNxtTools();
}

RobotsGeneratorPlugin::~RobotsGeneratorPlugin()
{
    delete mFlashTool;
}

void RobotsGeneratorPlugin::init(PluginConfigurator const &configurator)
{
    mMainWindowInterface = &configurator.mainWindowInterpretersInterface();
    mRepoControlApi = &configurator.repoControlInterface();

    mFlashTool = new NxtFlashTool(mMainWindowInterface->errorReporter());
}

QList<ActionInfo> RobotsGeneratorPlugin::actions()
{
    mGenerateCodeAction.setText(tr("Generate code"));
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

void RobotsGeneratorPlugin::changeActiveTab(QList<ActionInfo> const &info, bool const &trigger)
{
    foreach (ActionInfo const &actionInfo, info) {
            actionInfo.action()->setEnabled(trigger);
    }
}

void RobotsGeneratorPlugin::generateRobotSourceCode()
{
    robots::generator::NxtOSEKRobotGenerator gen(mMainWindowInterface->activeDiagram(),
             *mRepoControlApi,
             *mMainWindowInterface->errorReporter());
    mMainWindowInterface->errorReporter()->clearErrors();
    gen.generate();
    if (mMainWindowInterface->errorReporter()->wereErrors()) {
        return;
    }

    QFile file("nxt-tools/example0/example0.c");
    QTextStream *inStream = NULL;
    if (!file.isOpen() && file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        inStream = new QTextStream(&file);
    }

    if (inStream) {
        mMainWindowInterface->showInTextEditor("example0", inStream->readAll());
    }
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
        generateRobotSourceCode();
        mFlashTool->uploadProgram();
    }
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
