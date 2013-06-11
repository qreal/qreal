#include "trikGeneratorPlugin.h"

#include <QtWidgets/QApplication>

using namespace qReal;
using namespace robots::trikGenerator;

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

//	robots::generator::NxtOSEKRobotGenerator gen(mMainWindowInterface->activeDiagram(),
//			 *mRepoControlApi,
//			 *mMainWindowInterface->errorReporter());
	mMainWindowInterface->errorReporter()->clearErrors();
//	gen.generate();
	if (mMainWindowInterface->errorReporter()->wereErrors()) {
		return false;
	}

//	QFile file("nxt-tools/example0/example0.c");
//	QTextStream *inStream = NULL;
//	if (!file.isOpen() && file.open(QIODevice::ReadOnly | QIODevice::Text)) {
//		inStream = new QTextStream(&file);
//	}

//	if (inStream) {
//		mMainWindowInterface->showInTextEditor("example0", inStream->readAll());
//	}
	return true;
}

void TrikGeneratorPlugin::uploadProgram()
{
//	if (!mNxtToolsPresent) {
//		mMainWindowInterface->errorReporter()->addError(tr("flash.sh not found. Make sure it is present in QReal installation directory"));
//	} else {
//		mFlashTool->flashRobot();
//	}
}

void TrikGeneratorPlugin::runProgram()
{
//	if (!mNxtToolsPresent) {
//		mMainWindowInterface->errorReporter()->addError(tr("upload.sh not found. Make sure it is present in QReal installation directory"));
//	} else {
//		if (generateRobotSourceCode()) {
//			mFlashTool->uploadProgram();
//		}
//	}
}
