#pragma once

#include <QtCore/QString>
#include "../../qrgui/editorPluginInterface/editorInterface.h"

namespace editorPluginTestingFramework {

class PluginLoader
{
public:
	qReal::EditorInterface* loadedPlugin(QString const &fileName, QString const &pathToFile);

	QStringList pluginNames();

private:
	QStringList mPluginNames;
};

}
