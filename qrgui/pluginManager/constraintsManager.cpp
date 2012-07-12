#include "constraintsManager.h"

#include <QtCore/QDir>
#include <QtCore/QCoreApplication>
#include <QtCore/QPluginLoader>
#include <QtGui/QMessageBox>

using namespace qReal;

ConstraintsManager::ConstraintsManager()
{
	mPluginsDir = QDir(qApp->applicationDirPath());

	while (!mPluginsDir.isRoot() && !mPluginsDir.entryList(QDir::Dirs).contains("plugins")) {
		mPluginsDir.cdUp();
	}

	mPluginsDir.cd("plugins");

	foreach (QString fileName, mPluginsDir.entryList(QDir::Files)) {
		// TODO: Free memory
		QPluginLoader *loader  = new QPluginLoader(mPluginsDir.absoluteFilePath(fileName));
		QObject *plugin = loader->instance();

		if (plugin) {
			ConstraintsPluginInterface *constraintsPlugin = qobject_cast<ConstraintsPluginInterface *>(plugin);
			if (constraintsPlugin) {
//				mPlugins << constraintsPlugin;
				mPluginsLoaded += constraintsPlugin->id();
				mPluginFileName.insert(constraintsPlugin->id(), fileName);
				mLoaders.insert(fileName, loader);
				mPluginIface[constraintsPlugin->id()] = constraintsPlugin;
			}
			else {
				loader->unload();
				delete loader;
			}
		} else {
			qDebug() << "Plugin loading failed: " << loader->errorString();
			loader->unload();
			delete loader;
		}
	}
}

bool ConstraintsManager::loadPlugin(const QString &pluginName)
{
	QPluginLoader *loader = new QPluginLoader(mPluginsDir.absoluteFilePath(pluginName));
	QObject *plugin = loader->instance();

	if (plugin) {
		ConstraintsPluginInterface *constraintsPlugin = qobject_cast<ConstraintsPluginInterface *>(plugin);
		if (constraintsPlugin) {
//			mPlugins << constraintsPlugin;
			mPluginsLoaded += constraintsPlugin->id();
			mPluginFileName.insert(constraintsPlugin->id(), pluginName);
			mLoaders.insert(pluginName, loader);
			mPluginIface[constraintsPlugin->id()] = constraintsPlugin;
			return true;
		}
	}
	QMessageBox::warning(0, "QReal Plugin", loader->errorString());
	return false;
}

bool ConstraintsManager::unloadPlugin(const QString &pluginId)
{
	QString pluginName = mPluginFileName[pluginId];
	QPluginLoader *loader = mLoaders[pluginName];
	if (loader != NULL) {
		bool r = loader->unload();//qwerty_lsd ; don't work, why?
		if (!(r)) {
			return false;
		}
		mPluginsLoaded.removeAll(pluginId);
		mPluginFileName.remove(pluginId);
		return true;
	}
	return false;
}

IdList ConstraintsManager::pluginsIds() const
{
	IdList plugins;
	foreach (QString pluginId, mPluginsLoaded) {
		plugins.append(Id(pluginId));
	}
	return plugins;
}

QList<QString> ConstraintsManager::pluginsNames() const
{
	return mPluginFileName.values();
}

QList<CheckStatus> ConstraintsManager::check(Id const &element, qrRepo::LogicalRepoApi const &logicalApi, EditorManager const &editorManager)
{
	QList<qReal::CheckStatus> checkings;
	foreach (ConstraintsPluginInterface *constraintsInterface, mPluginIface.values()) {
		if (constraintsInterface->isCorrectMetamodelName(element)) {
			checkings.append(constraintsInterface->check(element, logicalApi, editorManager));
		}
	}
	return checkings;
}
