#pragma once

#include <QtCore/QString>
#include "../../qrgui/editorPluginInterface/editorInterface.h"

namespace editorPluginTestingFramework {

class PluginLoader
{
public:

	/// gets name of .dll and returns loaded plugin
	qReal::EditorInterface* loadedPlugin(QString const &fileName, QString const &pathToFile);

	/// returns generated plugins names
	QStringList pluginNames();

private:
	QStringList mPluginNames;
};

}
