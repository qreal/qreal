#pragma once

#include "mainwindow/mainWindow.h"
#include "mainwindow/palette/draggableElement.h"

namespace qReal {
namespace gui {

class GuiFacade
{
public:
	GuiFacade(MainWindow *mainWindow);

	QWidget* widget2dModel(QString const &type, QString const &name);
	QWidget* widget(QString const &type, QString const &name);
	DraggableElement* draggableElement(QString const &widgetId) const;
	ToolPluginInterface* plugin();

private:
	MainWindow *mMainWindow;
	ToolPluginInterface *mRobotsPlugin;
};
}
}
