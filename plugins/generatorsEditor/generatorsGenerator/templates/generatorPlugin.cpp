#include "@@generatorName@@Plugin.h"
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QProgressBar>
#include <QtCore/QProcess>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDesktopWidget>

#include "@@pathToQReal@@/qrkernel/settingsManager.h"

using namespace generators::@@generatorName@@;
using namespace qReal;

@@generatorClassName@@Plugin::@@generatorClassName@@Plugin()
{
}

@@generatorClassName@@Plugin::~@@generatorClassName@@Plugin()
{
}

void @@generatorClassName@@Plugin::init(PluginConfigurator const &configurator)
{
	mMainWindowInterface = &configurator.mainWindowInterpretersInterface();
	mLogicalModel = &configurator.logicalModelApi();
	mErrorReporter = configurator.mainWindowInterpretersInterface().errorReporter();
}

QList<ActionInfo> @@generatorClassName@@Plugin::actions()
{
	QAction * const generateAction = new QAction(QObject::tr("Generate @@generatorName@@"), NULL);
	ActionInfo generateActionInfo(generateAction, "interpreters", "tools");
	QObject::connect(generateAction, SIGNAL(triggered()), this, SLOT(generate()));

	return QList<ActionInfo>() << generateActionInfo;
}

void @@generatorClassName@@Plugin::generate()
{
	foreach (qReal::Id const &diagram, mLogicalModel->logicalRepoApi().elementsByType("@@languageNodeDiagram@@")) {
		if (!mLogicalModel->logicalRepoApi().isLogicalElement(diagram)) {
			continue;
		}

		QString programName = mLogicalModel->logicalRepoApi().property(diagram, "@@programNameProperty@@").toString();
		QString pathToGenerate = mLogicalModel->logicalRepoApi().property(diagram, "@@pathToGenerateProperty@@").toString();

		mGenerator = new Generator(pathToGenerate, programName, *mLogicalModel, *mErrorReporter);
		mGenerator->generate();
	}
}
