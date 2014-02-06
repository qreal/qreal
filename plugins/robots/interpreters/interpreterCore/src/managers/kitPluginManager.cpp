#include "kitPluginManager.h"

#include <QtCore/QFileInfo>

#include <qrkernel/exception/exception.h>

using namespace interpreterCore;

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
			interpreterBase::KitPluginInterface * const kitPlugin
					= qobject_cast<interpreterBase::KitPluginInterface *>(plugin);
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

interpreterBase::KitPluginInterface &KitPluginManager::kitById(QString const &kitId)
{
	if (!mPluginInterfaces.contains(kitId)) {
		throw qReal::Exception("Requesting non-existing kit plugin");
	}

	return *mPluginInterfaces[kitId];
}

void KitPluginManager::selectKit(QString const &kitId)
{
	if (!mPluginInterfaces.contains(kitId)) {
		throw qReal::Exception("Trying to select non-existing kit plugin");
	}

	mSelectedPlugin = mPluginInterfaces.value(kitId);
}

interpreterBase::KitPluginInterface &KitPluginManager::selectedKit()
{
	if (!mSelectedPlugin) {
		throw qReal::Exception("No kit selected");
	}

	return *mSelectedPlugin;
}
