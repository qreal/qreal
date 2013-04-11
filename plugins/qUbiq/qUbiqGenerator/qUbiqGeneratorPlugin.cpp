#include "qUbiqGeneratorPlugin.h"
#include <QtGui/QMessageBox>
#include <QtGui/QProgressBar>
#include <QtCore/QProcess>
#include <QtGui/QFileDialog>
#include <QtGui/QApplication>
#include <QtGui/QDesktopWidget>

#include "../../../qrkernel/settingsManager.h"

Q_EXPORT_PLUGIN2(QUbiqGeneratorPlugin, qUbiq::qUbiqGenerator::QUbiqGeneratorPlugin)

using namespace qUbiq::qUbiqGenerator;
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
//	mGenerator.init(configurator.logicalModelApi(), *configurator.mainWindowInterpretersInterface().errorReporter());
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
//	foreach (qReal::Id const &metamodel, mLogicalModel->logicalRepoApi().elementsByType("MetamodelConstraints")) {
//		if (!mLogicalModel->logicalRepoApi().isLogicalElement(metamodel)) {
//			continue;
//		}
//		mGenerator.generate(metamodel);
//	}

	mGenerator = new Generator(QString(), QString(), QString(), *mLogicalModel, *mErrorReporter, QString());
	mGenerator->generate();
}
