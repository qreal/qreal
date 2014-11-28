#pragma once

#include <QtWidgets/QWidget>
#include <QtWidgets/QTreeWidgetItem>
#include <QtWidgets/QScrollBar>

#include <plugins/toolPluginInterface/toolPluginInterface.h>

namespace qReal {

class MainWindow;

namespace gui {

class DraggableElement;

/// Implemented facade pattern. This class allows to search and get objects of graphical user interface of
/// system by theirs object names, parents and types.
class GuiFacade : public QObject
{
	Q_OBJECT

public:
	explicit GuiFacade(MainWindow *mainWindow);

	/// Returns widget defined by type(class name) and object name.
	Q_INVOKABLE QWidget *widget(QString const &type, QString const &name = "");

	/// Returns action belongs to plugin by object name.
	Q_INVOKABLE QWidget *pluginActionToolButton(QString const &name);

	/// Returns QRect of property widget by her name. Need to click on property editor, on property position
	/// before use property widget.
	Q_INVOKABLE QRect propertyRect(QString const &name);

	/// Returns widget by name from property editor tree widget.
	Q_INVOKABLE QWidget *property(QString const &name);

	/// Returns viewport(because scene viewport, not scene, recieve mouse event) of EditorViewScene of
	/// current tab.
	Q_INVOKABLE QWidget *sceneViewport();

	/// Returns main window.
	Q_INVOKABLE QWidget *mainWindow();

	/// Returns viewport(because viewport, not PropertyEditorView, recieve mouse event) PropertyEditorView.
	Q_INVOKABLE QWidget *propertyEditor();

	/// Returns draggable element from palette by id.
	DraggableElement *draggableElement(QString const &widgetId);

	/// Returns plugin gui facade;
	QObject *pluginGuiFacade(QString const &pluginName);

private:
	QTreeWidgetItem *propertyTreeWidgetItem(QString const &name);

	MainWindow *mMainWindow;
};
}
}
