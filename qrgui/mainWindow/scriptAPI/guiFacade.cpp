#include "guiFacade.h"

#include <plugins/toolPluginInterface/toolPluginInterface.h>
#include <editor/editorView.h>
#include <qrutils/widgetFinder.h>

#include "mainWindow.h"
#include "palette/draggableElement.h"

using namespace qReal;
using namespace gui;

GuiFacade::GuiFacade(MainWindow &mainWindow)
	: mMainWindow(mainWindow)
{
}

QWidget *GuiFacade::widget(QString const &type, QString const &name) const
{
	return utils::WidgetFinder::widget(&mMainWindow, type, name);
}

DraggableElement *GuiFacade::draggableElement(QString const &widgetId) const
{
	QList<DraggableElement *> const paletteWidgets = mMainWindow.findChildren<DraggableElement *>();
	for (DraggableElement * const paletteElement : paletteWidgets) {
		if (paletteElement->id().toString() == widgetId) {
			return paletteElement;
		}
	}

	return nullptr;
}

QRect GuiFacade::propertyRect(QString const &name) const
{
	QTreeWidget * const editorTree = mMainWindow.
			propertyEditor()->
			findChild<QtTreePropertyBrowser *>()->
			findChild<QTreeWidget *>();

	/// Adding 10 to coords needed to navigate in second column with interactive widgets.
	return QRect(editorTree->findChild<QWidget *>("qt_scrollarea_viewport")->mapTo(&mMainWindow
			, editorTree->visualItemRect(propertyTreeWidgetItem(name)).center() + QPoint(10, 0))
			, editorTree->visualItemRect(propertyTreeWidgetItem(name)).size());
}

QObject *GuiFacade::pluginGuiFacade(QString const &pluginName) const
{
	return mMainWindow.toolManager().pluginGuiFacade(pluginName);
}

QWidget *GuiFacade::property(QString const &name) const
{
	QTreeWidget * const editorTree = mMainWindow.
			propertyEditor()->
			findChild<QtTreePropertyBrowser *>()->
			findChild<QTreeWidget *>();
	return editorTree->itemWidget(propertyTreeWidgetItem(name), 1);
}

QWidget *GuiFacade::pluginActionToolButton(QString const &name) const
{
	const QList<ActionInfo> actionList = mMainWindow.toolManager().actions();
	for (const ActionInfo &actionInfo : actionList) {
		if (actionInfo.action()->objectName() == name) {
			const QList<QWidget *> widgetList = actionInfo.action()->associatedWidgets();
			for (QWidget * const widget : widgetList) {
				const QString buttonClassName = "QToolButton";
				if (buttonClassName == widget->metaObject()->className() && widget->isVisible()) {
					return widget;
				}
			}
		}
	}

	return nullptr;
}

QWidget *GuiFacade::sceneViewport() const
{
	return mMainWindow.getCurrentTab()->viewport();
}

QWidget *GuiFacade::mainWindow() const
{
	return &mMainWindow;
}

QWidget *GuiFacade::propertyEditor() const
{
	PropertyEditorView const *propertyEditor = mMainWindow.propertyEditor();
	return propertyEditor->
			findChild<QtTreePropertyBrowser *>()->
			findChild<QTreeWidget *>()->viewport();
}

QTreeWidgetItem *GuiFacade::propertyTreeWidgetItem(QString const &name) const
{
	QTreeWidget * const editorTree = mMainWindow.
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
