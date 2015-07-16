#include "pluginLoader.h"
#include <QtCore/QPluginLoader>
#include <QtCore/QDir>

#include "qrrepo/repoApi.h"
#include "qrutils/nameNormalizer.h"

using namespace qReal;
using namespace editorPluginTestingFramework;
using namespace utils;
using namespace qrRepo;

EditorInterface* PluginLoader::loadedPlugin(
		const QString &fileName
		, const QString &pathToFile
		, const QString &pluginExtension
		, const QString &prefix)
{
	qDebug() << "STARTING PLUGIN LOADING";
	QDir mPluginDir = QDir(pathToFile);

	QString normalizedFileName = fileName;
	if (!fileName.contains(".qrs")) {
		normalizedFileName += ".qrs";
	}

	const RepoApi *mRepoApi = new RepoApi(normalizedFileName);
	const IdList metamodels = mRepoApi->children(Id::rootId());

	for (Id const &key : metamodels) {
		if (mRepoApi->isLogicalElement(key)) {
			const QString &normalizedMetamodelName = NameNormalizer::normalize(mRepoApi->stringProperty(key, "name"), false);
			const QString &pluginName = prefix + normalizedMetamodelName + "-d" + "." + pluginExtension;
			mPluginNames.append(pluginName);

			QPluginLoader * const loader = new QPluginLoader(mPluginDir.absoluteFilePath(pluginName));
			loader->load();
			QObject *plugin = loader->instance();

			if (plugin) {
				qDebug() << "plugin is loaded";
				EditorInterface * const iEditor = qobject_cast<EditorInterface *>(plugin);
				return iEditor;
			}

			qDebug() << "plugin is NOT loaded";
		}
	}

	return NULL;
}

QStringList PluginLoader::pluginNames()
{
	return mPluginNames;
}
