#include "ubiqGeneratorPlugin.h"
#include <QMessageBox>
#include <QProgressBar>
#include <QtCore/QProcess>
#include <QFileDialog>
#include <QApplication>
#include <QDesktopWidget>

#include "../../../qrkernel/settingsManager.h"

using namespace ubiq::generator;
using namespace qReal;

QUbiqGeneratorPlugin::QUbiqGeneratorPlugin()
{
}

QUbiqGeneratorPlugin::~QUbiqGeneratorPlugin()
{
}

void QUbiqGeneratorPlugin::init(PluginConfigurator const &configurator)
{
	mMainWindowInterface = &configurator.mainWindowInterpretersInterface();
	mLogicalModel = &configurator.logicalModelApi();
	mErrorReporter = configurator.mainWindowInterpretersInterface().errorReporter();
}

QList<ActionInfo> QUbiqGeneratorPlugin::actions()
{
	QAction * const generateAction = new QAction(QObject::tr("Generate QUbiqProgram"), NULL);
	ActionInfo generateActionInfo(generateAction, "interpreters", "tools");
	QObject::connect(generateAction, SIGNAL(triggered()), this, SLOT(generate()));

	return QList<ActionInfo>() << generateActionInfo;
}

void QUbiqGeneratorPlugin::generate()
{
	foreach (qReal::Id const &diagram, mLogicalModel->logicalRepoApi().elementsByType("qUbiqPresentationDiagram")) {
		if (!mLogicalModel->logicalRepoApi().isLogicalElement(diagram)) {
			continue;
		}

		QString programName = mLogicalModel->logicalRepoApi().property(diagram, "programName").toString();
		QString pathToGenerate = mLogicalModel->logicalRepoApi().property(diagram, "pathToGenerate").toString();

		mGenerator = new Generator(pathToGenerate, programName, *mLogicalModel, *mErrorReporter);
		mGenerator->generate();
	}
}
