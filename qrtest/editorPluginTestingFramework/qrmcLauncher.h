#pragma once

#include <QtCore/QString>
#include "qrgui/toolPluginInterface/pluginConfigurator.h"

namespace editorPluginTestingFramework {

class QrmcLauncher
{
public:

	/// launches qrmc (gets fileName and path to qrmc-bin.pro and generates code)
	void launchQrmc(QString const &fileName, QString const &pathToQrmc, QString const &pathToGeneratedCode);
};

}
