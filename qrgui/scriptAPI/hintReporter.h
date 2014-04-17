#pragma once

#include <QtWidgets/QWidget>
#include "mainwindow/mainWindow.h"
#include "toolPluginInterface/usedInterfaces/errorReporterInterface.h"

namespace qReal {
namespace gui {

class HintReporter : public QLabel{
	Q_OBJECT

public:
	HintReporter(MainWindow *mainWindow);

public slots:
	void addHint(QString const &hint);

private:
	MainWindow *mMainWindow;
};

}
}
