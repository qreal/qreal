#include "nxtRussianCGeneratorPlugin.h"

#include <QtWidgets/QApplication>
#include <QtCore/QDir>

#include "nxtRussianCMasterGenerator.h"

using namespace nxt::russianC;

NxtRussianCGeneratorPlugin::NxtRussianCGeneratorPlugin()
	: NxtGeneratorPluginBase("NxtRussialCGeneratorRobotModel", tr("Generation (Russian C)"), 7 /* Last order */)
	, mGenerateCodeAction(new QAction(nullptr))
{
	mGenerateCodeAction->setText(tr("Generate to Russian C"));
	mGenerateCodeAction->setIcon(QIcon(":/nxt/russianC/images/generateRussianCCode.svg"));
	mGenerateCodeAction->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_G));
	connect(mGenerateCodeAction, SIGNAL(triggered()), this, SLOT(generateCode()));
}

QList<qReal::ActionInfo> NxtRussianCGeneratorPlugin::customActions()
{
	return { qReal::ActionInfo(mGenerateCodeAction, "generators", "tools") };
}

QList<qReal::HotKeyActionInfo> NxtRussianCGeneratorPlugin::hotKeyActions()
{
	return { qReal::HotKeyActionInfo("Generator.GenerateNxtRussianC"
			, tr("Generate Russian C Code"), mGenerateCodeAction) };
}

QIcon NxtRussianCGeneratorPlugin::iconForFastSelector(const kitBase::robotModel::RobotModelInterface &robotModel) const
{
	Q_UNUSED(robotModel)
	return QIcon(":/nxt/russianC/images/switch-to-nxt-russian-c.svg");
}

QString NxtRussianCGeneratorPlugin::defaultFilePath(const QString &projectName) const
{
	return QString::fromUtf8("russianC/%1/%1.си").arg(projectName);
}

qReal::text::LanguageInfo NxtRussianCGeneratorPlugin::language() const
{
	return qReal::text::Languages::russianC();
}

QString NxtRussianCGeneratorPlugin::generatorName() const
{
	return "nxtRussianC";
}

generatorBase::MasterGeneratorBase *NxtRussianCGeneratorPlugin::masterGenerator()
{
	return new NxtRussianCMasterGenerator(*mRepo
			, *mMainWindowInterface->errorReporter()
			, *mParserErrorReporter
			, *mRobotModelManager
			, *mTextLanguage
			, mMainWindowInterface->activeDiagram()
			, generatorName());
}

void NxtRussianCGeneratorPlugin::regenerateExtraFiles(const QFileInfo &newFileInfo)
{
	Q_UNUSED(newFileInfo);
}
