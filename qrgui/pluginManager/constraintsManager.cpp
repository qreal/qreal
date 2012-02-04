#include "constraintsManager.h"

#include <QtCore/QDir>
#include <QtCore/QCoreApplication>
#include <QtCore/QPluginLoader>

using namespace qReal;

ConstraintsManager::ConstraintsManager()
{
	QDir pluginsDir = QDir(qApp->applicationDirPath());

	while (!pluginsDir.isRoot() && !pluginsDir.entryList(QDir::Dirs).contains("plugins")) {
		pluginsDir.cdUp();
	}

	pluginsDir.cd("plugins");

	foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
		// TODO: Free memory
		QPluginLoader *loader  = new QPluginLoader(pluginsDir.absoluteFilePath(fileName));
		QObject *plugin = loader->instance();

		if (plugin) {
			ConstraintsPluginInterface *constraintsPlugin = qobject_cast<ConstraintsPluginInterface *>(plugin);
			if (constraintsPlugin) {
				mPlugins << constraintsPlugin;
//				mLoaders << loader;
			}
			else {
				delete loader;
			}
		} else {
			delete loader;
		}
	}
}

QPair<bool, QPair<QString, QString> > ConstraintsManager::check(IdList const &elements, qrRepo::LogicalRepoApi const &logicalApi)
{
	foreach (ConstraintsPluginInterface *constraintsInterface, mPlugins) {
		if (constraintsInterface->isCorrectLanguageName(elements)) {
//		if (constraintsInterface->languageName() == elements.at(0).editor()) {
			return constraintsInterface->check(elements, logicalApi);
		}
	}
	return QPair<bool, QPair<QString, QString> >(true, QPair<QString, QString>("", "information"));
}
