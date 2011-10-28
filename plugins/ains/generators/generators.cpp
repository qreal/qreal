#include "generators.h"

#include <QtCore/QTranslator>
#include <QtGui/QApplication>

Q_EXPORT_PLUGIN2(ainsPlugin, qReal::ains::AinsPlugin)

using namespace qReal;
using namespace ains;

AinsPlugin::AinsPlugin()
{
	QTranslator *appTranslator = new QTranslator();
	appTranslator->load(":/ains_" + QLocale::system().name());
	QApplication::installTranslator(appTranslator);
}

AinsPlugin::~AinsPlugin()
{
}

void AinsPlugin::init(PluginConfigurator const &configurator)
{
	mDatabaseEditorGenerator = new DatabaseEditorSchemeGenerator(configurator.logicalModelApi(), *configurator.mainWindowInterpretersInterface().errorReporter());
}

qReal::CustomizationInterface* AinsPlugin::customizationInterface()
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

