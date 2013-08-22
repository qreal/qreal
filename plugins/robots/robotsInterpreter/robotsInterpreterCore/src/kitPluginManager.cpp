#include "kitPluginManager.h"

#include <QtCore/QFileInfo>

#include <qrkernel/exception/exception.h>

using namespace robotsInterpreterCore;

KitPluginManager::KitPluginManager(QString const &pluginDirectory)
		: mPluginsDir(QDir(pluginDirectory))
{
	foreach (QString const &fileName, mPluginsDir.entryList(QDir::Files)) {
		QFileInfo const fileInfo(fileName);

		if (fileInfo.suffix() != "dll" && fileInfo.suffix() != "so") {
			continue;
		}

		QPluginLoader * const loader  = new QPluginLoader(mPluginsDir.absoluteFilePath(fileName));
		QObject * const plugin = loader->instance();

		if (plugin) {
			KitPluginInterface * const kitPlugin = qobject_cast<KitPluginInterface *>(plugin);
			if (kitPlugin) {
				mPluginFileNames.insert(kitPlugin->kitName(), fileName);
				mPluginInterfaces[kitPlugin->kitName()] = kitPlugin;
				mLoaders.insert(fileName, loader);
			} else {
				// loader->unload();
				delete loader;
			}
		} else {
			qDebug() << "Plugin loading failed: " << loader->errorString();
			loader->unload();
			delete loader;
		}
	}
}

KitPluginManager::~KitPluginManager()
{
	qDeleteAll(mPluginInterfaces);
	qDeleteAll(mLoaders);
}

QList<QString> KitPluginManager::kitNames() const
{
	return mPluginInterfaces.keys();
}

QWidget *KitPluginManager::kitSpecificSettingsWidget(QString const &kitName) const
{
	if (!mPluginInterfaces.contains(kitName)) {
		throw qReal::Exception("Trying to get settings widget for non-existing kit plugin");
	}

	return mPluginInterfaces[kitName]->settingsWidget();
}
