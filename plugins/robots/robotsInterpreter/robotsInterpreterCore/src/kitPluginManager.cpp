#include "kitPluginManager.h"

#include <QtCore/QFileInfo>

#include <qrkernel/exception/exception.h>

using namespace robotsInterpreterCore;

KitPluginManager::KitPluginManager(QString const &pluginDirectory)
		: mPluginsDir(QDir(pluginDirectory))
		, mSelectedPlugin(NULL)
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
				mPluginInterfaces[kitPlugin->kitId()] = kitPlugin;
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
	qDeleteAll(mLoaders);
}

QList<QString> KitPluginManager::kitIds() const
{
	return mPluginInterfaces.keys();
}

void KitPluginManager::selectKit(QString const &kitId)
{
	if (!mPluginInterfaces.contains(kitId)) {
		throw qReal::Exception("Trying to select non-existing kit plugin");
	}

	mSelectedPlugin = mPluginInterfaces.value(kitId);
}

KitPluginInterface &KitPluginManager::selectedKit()
{
	if (!mSelectedPlugin) {
		throw qReal::Exception("No kit selected");
	}

	return *mSelectedPlugin;
}
