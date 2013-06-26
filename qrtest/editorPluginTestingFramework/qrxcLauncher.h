#pragma once

#include <QtCore/QString>
#include "../../qrgui/mainwindow/mainWindowInterpretersInterface.h"

namespace editorPluginTestingFramework {

class QrxcLauncher
{
public:
	void launchQrxc(QString const &fileName);

private:
	static QString generatePathToPlugin(QString const &pathToQRealRoot);

	qReal::gui::MainWindowInterpretersInterface *mMainWindowInterface;
};

}
