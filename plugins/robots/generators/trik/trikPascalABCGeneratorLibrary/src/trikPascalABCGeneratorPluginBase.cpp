#include "trikPascalABCGeneratorLibrary/trikPascalABCGeneratorPluginBase.h"

#include <trikGeneratorBase/trikGeneratorPluginBase.h>
#include <trikGeneratorBase/robotModel/generatorModelExtensionInterface.h>
#include <utils/tcpRobotCommunicator.h>

#include "trikPascalABCMasterGenerator.h"

using namespace trik::pascalABC;
using namespace kitBase::robotModel;
using namespace qReal;

TrikPascalABCGeneratorPluginBase::TrikPascalABCGeneratorPluginBase(
		kitBase::robotModel::RobotModelInterface * const robotModel
		, kitBase::blocksBase::BlocksFactoryInterface * const blocksFactory
		, const QStringList &pathsToTemplates)
	: TrikGeneratorPluginBase(robotModel, blocksFactory)
	, mGenerateCodeAction(new QAction(nullptr))
	, mUploadProgramAction(new QAction(nullptr))
	, mRunProgramAction(new QAction(nullptr))
	, mStopRobotAction(new QAction(nullptr))
	, mPathsToTemplates(pathsToTemplates)
{
}

TrikPascalABCGeneratorPluginBase::~TrikPascalABCGeneratorPluginBase()
{
}

QList<ActionInfo> TrikPascalABCGeneratorPluginBase::customActions()
{
	mGenerateCodeAction->setObjectName("generatePascalABSCode");
	mGenerateCodeAction->setText(tr("Generate pascalABC code"));
	mGenerateCodeAction->setIcon(QIcon(":/trik/pascalABC/images/generateCode.svg"));
	ActionInfo generateCodeActionInfo(mGenerateCodeAction, "generators", "tools");
	connect(mGenerateCodeAction, SIGNAL(triggered()), this, SLOT(generateCode()), Qt::UniqueConnection);

	mUploadProgramAction->setObjectName("uploadProgram");
	mUploadProgramAction->setText(tr("Upload program"));
	mUploadProgramAction->setIcon(QIcon(":/trik/pascalABC/images/uploadProgram.svg"));
	ActionInfo uploadProgramActionInfo(mUploadProgramAction, "generators", "tools");
	connect(mUploadProgramAction, SIGNAL(triggered()), this, SLOT(uploadProgram()), Qt::UniqueConnection);

	mRunProgramAction->setObjectName("runProgram");
	mRunProgramAction->setText(tr("Run program"));
	mRunProgramAction->setIcon(QIcon(":/trik/pascalABC/images/run.png"));
	ActionInfo runProgramActionInfo(mRunProgramAction, "interpreters", "tools");
	connect(mRunProgramAction, SIGNAL(triggered()), this, SLOT(runProgram()), Qt::UniqueConnection);

	mStopRobotAction->setObjectName("stopRobot");
	mStopRobotAction->setText(tr("Stop robot"));
	mStopRobotAction->setIcon(QIcon(":/trik/pascalABC/images/stop.png"));
	ActionInfo stopRobotActionInfo(mStopRobotAction, "interpreters", "tools");
	connect(mStopRobotAction, SIGNAL(triggered()), this, SLOT(stopRobot()), Qt::UniqueConnection);

	return {generateCodeActionInfo, uploadProgramActionInfo, runProgramActionInfo, stopRobotActionInfo};
}

///ToDo hotKeyActions
QList<qReal::HotKeyActionInfo> TrikPascalABCGeneratorPluginBase::hotKeyActions()
{
	HotKeyActionInfo generateCodeInfo("Generator.GenerateTrik", tr("Generate TRIK Code"), mGenerateCodeAction);
	HotKeyActionInfo uploadProgramInfo("Generator.UploadTrik", tr("Upload TRIK Program"), mUploadProgramAction);
	HotKeyActionInfo runProgramInfo("Generator.RunTrik", tr("Run TRIK Program"), mRunProgramAction);
	HotKeyActionInfo stopRobotInfo("Generator.StopTrik", tr("Stop TRIK Robot"), mStopRobotAction);

	return {generateCodeInfo, uploadProgramInfo, runProgramInfo, stopRobotInfo};
}

QIcon TrikPascalABCGeneratorPluginBase::iconForFastSelector(const RobotModelInterface &robotModel) const
{
	Q_UNUSED(robotModel)
	return QIcon(":/trik/pascalABC/images/switch-to-trik-pascal.svg");
}

generatorBase::MasterGeneratorBase *TrikPascalABCGeneratorPluginBase::masterGenerator()
{
	return new TrikPascalABCMasterGenerator(*mRepo
			, *mMainWindowInterface->errorReporter()
			, *mParserErrorReporter
			, *mRobotModelManager
			, *mTextLanguage
			, mMainWindowInterface->activeDiagram()
			, mPathsToTemplates);
}

QString TrikPascalABCGeneratorPluginBase::defaultFilePath(const QString &projectName) const
{
	return QString("trik/%1/%1.pas").arg(projectName);
}

text::LanguageInfo TrikPascalABCGeneratorPluginBase::language() const
{
	return qReal::text::Languages::pascalABC({"robot"});
}

QString TrikPascalABCGeneratorPluginBase::generatorName() const
{
	return "trikPascalABC";
}

///ToDo Upload, Run, Stop
bool TrikPascalABCGeneratorPluginBase::uploadProgram()
{
	return true;
}

void TrikPascalABCGeneratorPluginBase::runProgram()
{
}

void TrikPascalABCGeneratorPluginBase::stopRobot()
{
}
