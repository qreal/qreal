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
	explicit GuiFacade(MainWindow *mainWindow);

	/// Return widget defined by type(class name) and object name.
	Q_INVOKABLE QWidget *widget(QString const &type, QString const &name = "");

	/// Return action belongs to plugin by object name.
	Q_INVOKABLE QWidget *pluginAction(QString const &name);

	/// Return QRect of property widget by her name. Need to click on property editor, on property position
	/// before use property widget.
	Q_INVOKABLE QRect propertyRect(QString const &name);

	/// Return combo box widget from property editor.
	Q_INVOKABLE QWidget *property(QString const &type, QString const &name);

	/// Return viewport(because scene viewport, not scene, recieve mouse event) of EditorViewScene of
	/// current tab.
	Q_INVOKABLE QWidget *scene();

	/// Return main window.
	Q_INVOKABLE QWidget *mainWindow();

	/// Return viewport(because viewport, not PropertyEditorView, recieve mouse event) PropertyEditorView.
	Q_INVOKABLE QWidget *propertyEditor();

	/// Return draggable element from palette by id.
	DraggableElement *draggableElement(QString const &widgetId);

	/// Return plugin gui facade;
	QObject *pluginGuiFacade(QString const &pluginName);

private:
	MainWindow *mMainWindow;
};
}
}
