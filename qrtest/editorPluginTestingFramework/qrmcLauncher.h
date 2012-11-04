#include <QtCore/QString>
#include "../../../qrgui/toolPluginInterface/pluginConfigurator.h"

class QrmcLauncher
{
public:
	QrmcLauncher();

	void launchQrmc(QString fileName, QString const directoryToGeneratedCode, QString const pathToQrmc);
};
