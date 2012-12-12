#pragma once

#include <QtCore/QString>
#include "../../qrgui/editorPluginInterface/editorInterface.h"

namespace editorPluginTestingFramework {

class PluginLoader
{
public:
	static qReal::EditorInterface* loadedPlugin(QString const &fileName, QString const &pathToFile);
};

}
