#pragma once

#include "mainwindow/mainWindow.h"

namespace qReal {
namespace gui {

class GuiFacade
{
public:
	GuiFacade(MainWindow *mainWindow);

	QWidget *getWidgetRef(QString const &type, QString const &name, QString const &widgetName) const;

private:
	MainWindow *mMainWindow;
};
}
}
