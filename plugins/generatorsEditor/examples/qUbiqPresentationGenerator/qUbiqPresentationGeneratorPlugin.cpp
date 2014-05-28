#include "qUbiqPresentationGeneratorPlugin.h"
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QProgressBar>
#include <QtCore/QProcess>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDesktopWidget>

#include "D:/qrealGen/qrkernel/settingsManager.h"

using namespace generators::qUbiqPresentationGenerator;
using namespace qReal;

QUbiqPresentationGeneratorPlugin::QUbiqPresentationGeneratorPlugin()
{
}

QUbiqPresentationGeneratorPlugin::~QUbiqPresentationGeneratorPlugin()
{
}

void QUbiqPresentationGeneratorPlugin::init(PluginConfigurator const &configurator)
{
	mMainWindowInterface = &configurator.mainWindowInterpretersInterface();
	mLogicalModel = &configurator.logicalModelApi();
	mErrorReporter = configurator.mainWindowInterpretersInterface().errorReporter();
}

QList<ActionInfo> QUbiqPresentationGeneratorPlugin::actions()
{
	QAction * const generateAction = new QAction(QObject::tr("Generate qUbiqPresentationDiagram"), NULL);
	ActionInfo generateActionInfo(generateAction, "interpreters", "tools");
	QObject::connect(generateAction, SIGNAL(triggered()), this, SLOT(generate()));

	return QList<ActionInfo>() << generateActionInfo;
}

void QUbiqPresentationGeneratorPlugin::generate()
{
	foreach (qReal::Id const &diagram, mLogicalModel->logicalRepoApi().elementsByType("qUbiqPresentationDiagram")) {
		if (!mLogicalModel->logicalRepoApi().isLogicalElement(diagram)) {
			continue;
		}

		QMap<QString, QString> marksProperty;
		QString programName = mLogicalModel->logicalRepoApi().property(diagram, "programName").toString();
		QString pathToGenerate = mLogicalModel->logicalRepoApi().property(diagram, "pathToGenerate").toString();

		mGenerator = new Generator(pathToGenerate, programName, *mLogicalModel, *mErrorReporter, marksProperty);
		mGenerator->generate();
	}
}
