#pragma once

#include <QtCore/QString>
#include "qrgui/mainwindow/mainWindowInterpretersInterface.h"

namespace editorPluginTestingFramework {

class QrxcLauncher
{
public:

	/// launches qrxc (gets fileName, generates .xml-file and code)
	void launchQrxc(QString const &fileName, QString const &pathToQRealSources);

private:

	/// generates path to plugin from .pro
	static QString generatePathToPlugin();

	qReal::gui::MainWindowInterpretersInterface *mMainWindowInterface;
};

}
