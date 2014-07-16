#pragma once

#include <QtWidgets/QWidget>

#include "toolPluginInterface/toolPluginInterface.h"

namespace qReal {

class MainWindow;

namespace gui {

class DraggableElement;

/// Implemented facade pattern. This class allows to search and get ojects of graphical user interface of
/// system by theirs object names, parents and types.

class GuiFacade : public QObject
{
	Q_OBJECT

public:
	GuiFacade(MainWindow *mainWindow);

	/// Return widget defined by type(parent class, without character "Q". Like Action, Scene) and object name.
	Q_INVOKABLE QWidget* widget(QString const &type, QString const &name = "");

	/// Return QRect of property widget by her name.
	Q_INVOKABLE QRect property(QString const &name);

	/// Return draggable element from palette by id.
	DraggableElement* draggableElement(QString const &widgetId);

	/// Return plugin gui facade;
	ToolPluginInterface* plugin(QString const &pluginName);

private:
	QWidget *comboBoxProperty(QString const &name);
	QWidget *robotAction(QString const &name);
	MainWindow *mMainWindow;
	ToolPluginInterface *mPlugin;
};
}
}
