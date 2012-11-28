#include "pluginLoader.h"
#include <QtCore/QPluginLoader>
#include <QtCore/QDir>

PluginLoader::PluginLoader()
{
}

EditorInterface* PluginLoader::loadedPlugin(QString const &pluginName, QString const &pathToFile)
{
	QDir mPluginDir = QDir(pathToFile);

	QString formattedPluginName = pluginName + ".dll";
	QPluginLoader *loader = new QPluginLoader(mPluginDir.absoluteFilePath(formattedPluginName));
	qDebug() << mPluginDir.absoluteFilePath(formattedPluginName);
	loader->load();
	QObject *plugin = loader->instance();

	if (plugin) {
		qDebug() << "plugin is loaded";
		EditorInterface *iEditor = qobject_cast<EditorInterface *>(plugin);
		return iEditor;
	}
	qDebug() << "plugin is NOT loaded, Ffffffuuu";
	return NULL;
}
