#pragma once

#include <QtCore/QString>
#include "../../qrgui/mainwindow/mainWindowInterpretersInterface.h"

namespace editorPluginTestingFramework {

class QrxcLauncher
{
public:

	/// launches qrxc (gets fileName, generates .xml-file and code)
	void launchQrxc(QString const &fileName);

private:

	/// generates path to plugin from editorsSdk
	static QString generatePathToPlugin(QString const &pathToQRealRoot);

	qReal::gui::MainWindowInterpretersInterface *mMainWindowInterface;
};

}
