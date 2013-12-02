#include "ubiqGeneratorPlugin.h"
#include <QMessageBox>
#include <QProgressBar>
#include <QtCore/QProcess>
#include <QFileDialog>
#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>

#include <qrkernel/settingsManager.h>

using namespace ubiq::generator;
using namespace qReal;

UbiqGeneratorPlugin::UbiqGeneratorPlugin()
{
}

UbiqGeneratorPlugin::~UbiqGeneratorPlugin()
{
}

void UbiqGeneratorPlugin::init(PluginConfigurator const &configurator)
{
	mMainWindowInterface = &configurator.mainWindowInterpretersInterface();
	mLogicalModel = &configurator.logicalModelApi();
	mErrorReporter = configurator.mainWindowInterpretersInterface().errorReporter();
    mGraphicalModel = &configurator.graphicalModelApi();
}

QList<ActionInfo> UbiqGeneratorPlugin::actions()
{
    QAction * const generateAction = new QAction(QObject::tr("Generate UbiqProgram"), NULL);
	ActionInfo generateActionInfo(generateAction, "interpreters", "tools");
	QObject::connect(generateAction, SIGNAL(triggered()), this, SLOT(generate()));

	return QList<ActionInfo>() << generateActionInfo;
}

void UbiqGeneratorPlugin::generate()
{
//		QString programName = mLogicalModel->logicalRepoApi().property(diagram, "programName").toString();
//		QString pathToGenerate = mLogicalModel->logicalRepoApi().property(diagram, "pathToGenerate").toString();

        mGenerator = new Generator("./pathToGenerate", "programName", *mLogicalModel, *mGraphicalModel, *mErrorReporter);
        mGenerator->generate();
}
