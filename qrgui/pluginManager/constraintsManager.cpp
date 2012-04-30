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
//				mPluginsLoaded += constraintsPlugin->id();//qwerty_lsd
//				mPluginFileName.insert(constraintsPlugin->id(), pluginName);
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

//bool ConstraintsManager::unloadPlugin(const QString &pluginName)
//{
//	QPluginLoader *loader = mLoaders[mPluginFileName[pluginName]];
//	if (loader != NULL) {
//		if (!(loader->unload())) {
//			return false;
//		}
//		mPluginsLoaded.removeAll(pluginName);
//		mPluginFileName.remove(pluginName);
//		return true;
//	}
//	return false;
//}

CheckStatus ConstraintsManager::check(Id const &element, qrRepo::LogicalRepoApi const &logicalApi, EditorManager const &editorManager)
{
	foreach (ConstraintsPluginInterface *constraintsInterface, mPlugins) {
		if (constraintsInterface->isCorrectLanguageName(element)) {
			return constraintsInterface->check(element, logicalApi, editorManager);
		}
	}
	return CheckStatus(true, "", CheckStatus::warning);
}
