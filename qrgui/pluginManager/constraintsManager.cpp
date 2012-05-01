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
				mPlugins << constraintsPlugin;
				mPluginsLoaded += constraintsPlugin->id();
				mPluginFileName.insert(constraintsPlugin->id(), fileName);
				mLoaders.insert(fileName, loader);
			}
			else {
				delete loader;
			}
		} else {
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
			mPlugins << constraintsPlugin;
			mPluginsLoaded += constraintsPlugin->id();
			mPluginFileName.insert(constraintsPlugin->id(), pluginName);
			mLoaders.insert(pluginName, loader);
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

IdList ConstraintsManager::plugins() const
{
	IdList plugins;
	foreach (QString pluginId, mPluginsLoaded) {
		plugins.append(Id(pluginId));
	}
	return plugins;
}

CheckStatus ConstraintsManager::check(Id const &element, qrRepo::LogicalRepoApi const &logicalApi, EditorManager const &editorManager)
{
	foreach (ConstraintsPluginInterface *constraintsInterface, mPlugins) {
		if (constraintsInterface->isCorrectLanguageName(element)) {
			return constraintsInterface->check(element, logicalApi, editorManager);
		}
	}
	return CheckStatus(true, "", CheckStatus::warning);
}
