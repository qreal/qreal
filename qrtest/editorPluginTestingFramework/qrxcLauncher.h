#pragma once

#include <QtCore/QString>
#include "../../qrgui/mainwindow/mainWindowInterpretersInterface.h"

namespace editorPluginTestingFramework {

class QrxcLauncher
{
public:
	void launchQrxc(QString const &fileName);

private:
	qReal::gui::MainWindowInterpretersInterface *mMainWindowInterface;
};

}
