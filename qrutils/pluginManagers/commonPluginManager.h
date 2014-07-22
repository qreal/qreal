#pragma once

#include <QtCore/QString>
#include <QtCore/QDir>
#include <QtCore/QPluginLoader>
#include <QtCore/QObject>
#include <QtCore/QMap>

namespace qReal {

class CommonPluginManager
{
public:
	CommonPluginManager(QString const &applicationDirPath);
	~CommonPluginManager();

	/// deletes all loaders
	void deleteAllLoaders();
	/// returns list of all found plugins if succeed and empty list otherwise
	QList<QObject *> allLoadedPlugins();
	/// returns plugin found by name if succeed and nothing otherwise
	QObject *pluginLoadedByName(QString const &pluginName);

	/// unloads plugins, given FILENAME
	/// first member in pair is result of unloading
	/// second result is true if we found needed loader and false otherwise
	QPair<bool, bool> unloadPlugin(QString const &pluginName);

	/// returns fileName by given object
	QString fileName(QObject *plugin) const;

private:
	/// directory to loaded plugins
	QDir mPluginsDir;
	/// map from name to loader
	QMap<QString, QPluginLoader *> mLoaders;
	/// map from fileName to plugin
	QMap<QString, QObject *> mNameAndObject;
	/// path to application directory, used to count path to loaded plugins
	QString mApplicationDirectoryPath;
};
}
