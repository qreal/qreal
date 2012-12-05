#include "pluginLoader.h"
#include <QtCore/QPluginLoader>
#include <QtCore/QDir>

using namespace qReal;
using namespace editorPluginTestingFramework;

EditorInterface* PluginLoader::loadedPlugin(QString const &pluginName, QString const &pathToFile)
{
	QDir mPluginDir = QDir(pathToFile);

	QString const formattedPluginName = pluginName + ".dll";
	QPluginLoader * const loader = new QPluginLoader(mPluginDir.absoluteFilePath(formattedPluginName));
	qDebug() << mPluginDir.absoluteFilePath(formattedPluginName);
	loader->load();
	QObject *plugin = loader->instance();

	if (plugin) {
		qDebug() << "plugin is loaded";
		EditorInterface * const iEditor = qobject_cast<EditorInterface *>(plugin);
		return iEditor;
	}
	qDebug() << "plugin is NOT loaded";
	return NULL;
}
