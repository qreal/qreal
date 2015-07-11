#pragma once

#include <QtCore/QString>
#include "qrgui/plugins/toolPluginInterface/usedInterfaces/mainWindowInterpretersInterface.h"

namespace editorPluginTestingFramework {

class QrxcLauncher
{
public:

	/// launches qrxc (gets fileName, generates .xml-file and code)
	void launchQrxc(const QString &fileName, const QString &pathToQRealSources, const QString &pathToGeneratedCode);

private:

	/// generates path to plugin from .pro
	static QString generatePathToPlugin(const QString &pathToGeneratedCode);

	qReal::gui::MainWindowInterpretersInterface *mMainWindowInterface;
};

}
