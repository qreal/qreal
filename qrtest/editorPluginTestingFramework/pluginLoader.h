#pragma once

#include <QtCore/QString>
#include "../../qrgui/editorPluginInterface/editorInterface.h"

namespace editorPluginTestingFramework {

class PluginLoader
{
public:
	static qReal::EditorInterface* loadedPlugin(QString const &pluginName, QString const &pathToFile);
};

}
