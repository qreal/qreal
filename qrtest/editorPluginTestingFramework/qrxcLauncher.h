#pragma once

#include <QtCore/QString>
#include "qrgui/plugins/toolPluginInterface/usedInterfaces/mainWindowInterpretersInterface.h"

namespace editorPluginTestingFramework {

class QrxcLauncher
{
public:

	/// launches qrxc (gets fileName, generates .xml-file and code)
	void launchQrxc(QString const &fileName, QString const &pathToQRealSources, QString const &pathToGeneratedCode);

private:

	/// generates path to plugin from .pro
	static QString generatePathToPlugin(QString const &pathToGeneratedCode);

	qReal::gui::MainWindowInterpretersInterface *mMainWindowInterface;
};

}
