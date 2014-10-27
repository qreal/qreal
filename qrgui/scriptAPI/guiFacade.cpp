#include "guiFacade.h"

#include "mainwindow/mainWindow.h"
#include "mainwindow/palette/draggableElement.h"
#include "toolPluginInterface/toolPluginInterface.h"

using namespace qReal;
using namespace gui;

GuiFacade::GuiFacade(MainWindow *mainWindow)
	: mMainWindow(mainWindow)
{
}

QWidget *GuiFacade::widget(QString const &type, QString const &name)
{
	QList<QWidget *> const widgetList = mMainWindow->findChildren<QWidget *>(name);

	for (QWidget * const widget : widgetList) {
		if (type == widget->metaObject()->className() && widget->isVisible()) {
			return widget;
		}
	}

	return nullptr;
}

DraggableElement *GuiFacade::draggableElement(QString const &widgetId)
{
	QList<DraggableElement *> const paletteWidgets = mMainWindow->findChildren<DraggableElement *>();
	for (DraggableElement * const paletteElement : paletteWidgets) {
		if (paletteElement->id().toString() == widgetId) {
			return paletteElement;
		}
	}

	return nullptr;
}

QRect GuiFacade::propertyRect(QString const &name)
{
	QTreeWidget * const editorTree = mMainWindow->
			propertyEditor()->
			findChild<QtTreePropertyBrowser *>()->
			findChild<QTreeWidget *>();
	QRect const globalTarget = QRect(editorTree->findChild<QWidget *>("qt_scrollarea_viewport")->mapTo(mMainWindow,
			editorTree->visualItemRect(propertyTreeWidgetItem(name)).center() + QPoint(10, 0))
			, editorTree->visualItemRect(propertyTreeWidgetItem(name)).size());
	return globalTarget;
}

QObject *GuiFacade::pluginGuiFacade(QString const &pluginName)
{
	return mMainWindow->toolManager().pluginGuiFacade(pluginName);
}

QWidget *GuiFacade::property(QString const &name)
{
	QTreeWidget * const editorTree = mMainWindow->
			propertyEditor()->
			findChild<QtTreePropertyBrowser *>()->
			findChild<QTreeWidget *>();
	return editorTree->itemWidget(propertyTreeWidgetItem(name), 1);

	return nullptr;
}

QWidget *GuiFacade::pluginActionToolButton(QString const &name)
{
	QList<ActionInfo > actionList = mMainWindow->toolManager().actions();
	for(ActionInfo const &actionInfo : actionList) {
		if (actionInfo.action()->objectName() == name) {
			QList<QWidget *> const widgetList = actionInfo.action()->associatedWidgets();
			for (QWidget * const widget : widgetList) {
				QString const buttonClassName = "QToolButton";
				if (buttonClassName == widget->metaObject()->className() && widget->isVisible()) {
					return widget;
				}
			}
		}
	}

	return nullptr;
}

QWidget *GuiFacade::sceneViewport()
{
	return mMainWindow->getCurrentTab()->viewport();
}

QWidget *GuiFacade::mainWindow()
{
	return mMainWindow;
}

QWidget *GuiFacade::propertyEditor()
{
	PropertyEditorView const *propertyEditor = mMainWindow->propertyEditor();
	return propertyEditor->
			findChild<QtTreePropertyBrowser *>()->
			findChild<QTreeWidget *>()->viewport();
}

QTreeWidgetItem *GuiFacade::propertyTreeWidgetItem(QString const &name)
{
	QTreeWidget * const editorTree = mMainWindow->
			propertyEditor()->
			findChild<QtTreePropertyBrowser *>()->
			findChild<QTreeWidget *>();
	for (int i = 0; i < editorTree->topLevelItemCount(); ++i) {
		QTreeWidgetItem * const item = editorTree->topLevelItem(i);
		if (item->data(0, Qt::DisplayRole).toString() == name) {
			return item;
		}
	}

	return nullptr;
}
