#include <QtCore/QString>
#include "../../qrgui/mainwindow/mainWindowInterpretersInterface.h"
//#include "pluginCompiler.h"

using namespace qReal;
using namespace gui;

class QrxcLauncher
{
public:
	QrxcLauncher();

	void launchQrxc(QString &fileName);

private:
	MainWindowInterpretersInterface *mMainWindowInterface;
	//PluginCompiler mPluginCompiler;
};

