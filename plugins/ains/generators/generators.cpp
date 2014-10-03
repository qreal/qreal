#include "generators.h"

#include <QtWidgets/QApplication>

using namespace qReal;
using namespace ains;

AinsPlugin::AinsPlugin()
{
}

AinsPlugin::~AinsPlugin()
{
}

void AinsPlugin::init(PluginConfigurator const &configurator)
{
	mDatabaseEditorGenerator = new DatabaseEditorSchemeGenerator(configurator.logicalModelApi(), *configurator.mainWindowInterpretersInterface().errorReporter());
}

qReal::Customizer* AinsPlugin::customizationInterface()
{
	return NULL;
}

QList<ActionInfo> AinsPlugin::actions()
{
	mGenerateDatabaseSchemeAction = new QAction (QObject::tr("generate Database Scheme"), NULL);
	ActionInfo DatabaseSchemeActionInfo(mGenerateDatabaseSchemeAction, "interpreters", "tools");
	QObject::connect(mGenerateDatabaseSchemeAction, SIGNAL(triggered()), this, SLOT(generateDatabaseScheme()));

	QAction *separator = new QAction(NULL);
	ActionInfo separatorActionInfo(separator, "interpreters", "tools");
	separator->setSeparator(true);

	updateSettings();

	return QList<ActionInfo>() << DatabaseSchemeActionInfo << separatorActionInfo;
}

QPair<QString, PreferencesPage *> AinsPlugin::preferencesPage()
{
	return QPair<QString, PreferencesPage *>(QString(), NULL);
}

void AinsPlugin::updateSettings()
{

}

void AinsPlugin::generateDatabaseScheme()
{

	QHash<Id, QPair<QString, QString> > modelList = mDatabaseEditorGenerator->modelList();
	foreach (Id id, modelList.keys()) {
		QString path = modelList[id].first + "/" + modelList[id].second;
		mDatabaseEditorGenerator->generateDatabaseScheme(id, path);
	}
}

