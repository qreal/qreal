
#include <QtCore/QString>
#include "../../qrgui/editorPluginInterface/editorInterface.h"

class PluginLoader
{
public:
	PluginLoader();

	qReal::EditorInterface* loadedPlugin(QString const &pluginName, QString const &pathToFile);
};

