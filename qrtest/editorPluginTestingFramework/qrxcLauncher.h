#include <QtCore/QString>
#include "../../qrgui/mainwindow/mainWindowInterpretersInterface.h"

using namespace qReal;
using namespace gui;

class QrxcLauncher
{
public:
	QrxcLauncher();

	void launchQrxc(QString &fileName);

private:
	MainWindowInterpretersInterface *mMainWindowInterface;
};
