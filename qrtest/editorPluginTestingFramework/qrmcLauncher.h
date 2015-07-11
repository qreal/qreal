#pragma once

#include <QtCore/QString>
#include "qrgui/plugins/toolPluginInterface/pluginConfigurator.h"

namespace editorPluginTestingFramework {

class QrmcLauncher
{
public:
	/// launches qrmc (gets fileName and path to qrmc-bin.pro and generates code)
	void launchQrmc(const QString &fileName, const QString &pathToQrmc, const QString &pathToGeneratedCode);
};

}
