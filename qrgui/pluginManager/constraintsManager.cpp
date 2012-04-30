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
		mLoaders.insert(fileName, loader);
		QObject *plugin = loader->instance();

		if (plugin) {
			ConstraintsPluginInterface *constraintsPlugin = qobject_cast<ConstraintsPluginInterface *>(plugin);
			if (constraintsPlugin) {
				mPlugins << constraintsPlugin;
				mPluginsLoaded += constraintsPlugin->id();//qwerty_lsd
				mPluginFileName.insert(constraintsPlugin->id(), fileName);
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
	mLoaders.insert(pluginName, loader);
	QObject *plugin = loader->instance();

	if (plugin) {
		ConstraintsPluginInterface *constraintsPlugin = qobject_cast<ConstraintsPluginInterface *>(plugin);
		if (constraintsPlugin) {
			mPluginsLoaded += constraintsPlugin->id();
			mPluginFileName.insert(constraintsPlugin->id(), pluginName);
			return true;
		}
	}
	QMessageBox::warning(0, "QReal Plugin", loader->errorString());
	return false;
}

bool ConstraintsManager::unloadPlugin(const QString &pluginName)
{
	QPluginLoader *loader = mLoaders[mPluginFileName[pluginName]];
	if (loader != NULL) {
		if (!(loader->unload())) {
			return false;
		}
		mPluginsLoaded.removeAll(pluginName);
		mPluginFileName.remove(pluginName);
		return true;
	}
	return false;
}

IdList ConstraintsManager::plugins() const
{
	IdList plugins;
	foreach (QString plugin, mPluginsLoaded) {
		plugins.append(Id(plugin));
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
