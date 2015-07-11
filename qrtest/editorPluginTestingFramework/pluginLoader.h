#pragma once

#include <QtCore/QString>
#include "qrgui/plugins/editorPluginInterface/editorInterface.h"

namespace editorPluginTestingFramework {

class PluginLoader
{
public:

	/// gets name of dynamic library and returns loaded plugin
	qReal::EditorInterface* loadedPlugin(
			const QString &fileName
			, const QString &pathToFile
			, const QString &pluginExtension
			, const QString &prefix);

	/// returns generated plugins names
	QStringList pluginNames();

private:
	QStringList mPluginNames;
};

}
