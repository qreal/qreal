#include "pluginLoader.h"
#include <QtCore/QPluginLoader>
#include <QtCore/QDir>
#include "../../qrrepo/repoApi.h"

using namespace qReal;
using namespace editorPluginTestingFramework;

EditorInterface* PluginLoader::loadedPlugin(QString const &fileName, QString const &pathToFile)
{
	QDir mPluginDir = QDir(pathToFile);

	QString normalizedFileName = fileName;
	if (!fileName.contains(".qrs")) {
		normalizedFileName += ".qrs";
	}

	qrRepo::RepoApi *const mRepoApi = new qrRepo::RepoApi(normalizedFileName);

	IdList const metamodels = mRepoApi->children(Id::rootId());

	foreach (Id const &key, metamodels) {
		QString const &pluginName = mRepoApi->stringProperty(key, "name") + ".dll";
		QPluginLoader * const loader = new QPluginLoader(mPluginDir.absoluteFilePath(pluginName));
		qDebug() << mPluginDir.absoluteFilePath(pluginName);
		loader->load();
		QObject *plugin = loader->instance();

		if (plugin) {
			qDebug() << "plugin is loaded";
			EditorInterface * const iEditor = qobject_cast<EditorInterface *>(plugin);
			return iEditor;
		}
		qDebug() << "plugin is NOT loaded";
	}
	return NULL;
}
