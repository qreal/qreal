
#include <QtCore/QString>
#include "../../qrgui/editorPluginInterface/editorInterface.h"

using namespace qReal;
class PluginLoader
{
public:
	PluginLoader();

	EditorInterface* loadedPlugin(QString const &pluginName, QString const &pathToApp);
};

