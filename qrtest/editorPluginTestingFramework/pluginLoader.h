#pragma once

#include <QtCore/QString>
#include "qrgui/plugins/editorPluginInterface/editorInterface.h"

namespace editorPluginTestingFramework {

class PluginLoader
{
public:

	/// gets name of dynamic library and returns loaded plugin
	qReal::EditorInterface* loadedPlugin(
			QString const &fileName
			, QString const &pathToFile
			, QString const &pluginExtension
			, QString const &prefix);

	/// returns generated plugins names
	QStringList pluginNames();

private:
	QStringList mPluginNames;
};

}
