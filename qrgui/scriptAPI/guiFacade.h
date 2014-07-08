#pragma once

#include "mainwindow/palette/draggableElement.h"

namespace qReal {
namespace gui {

class GuiFacade : public QObject
{
	Q_OBJECT

public:
	GuiFacade(MainWindow *mainWindow);

	Q_INVOKABLE QWidget* widget(QString const &type, QString const &name = "");
	Q_INVOKABLE QRect property(QString const &name);
	DraggableElement* draggableElement(QString const &widgetId);
	ToolPluginInterface* plugin();

private:
	QWidget *comboBoxProperty(QString const &name);
	QWidget *robotAction(QString const &name);
	MainWindow *mMainWindow;
	ToolPluginInterface *mRobotsPlugin;
};
}
}
