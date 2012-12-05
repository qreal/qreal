#include <QtCore/QString>
#include "../../qrgui/mainwindow/mainWindowInterpretersInterface.h"

class QrxcLauncher
{
public:
	QrxcLauncher();

	void launchQrxc(QString &fileName);

private:
	qReal::gui::MainWindowInterpretersInterface *mMainWindowInterface;
};
