#pragma once

#include <QtCore/QString>
#include "../../../qrgui/toolPluginInterface/pluginConfigurator.h"

namespace editorPluginTestingFramework {

class QrmcLauncher
{
public:
	void launchQrmc(QString const &fileName, QString const &pathToQrmc);
};

}
