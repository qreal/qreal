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

	for (QWidget *widget : widgetList) {
		if (type == widget->metaObject()->className() && widget->isVisible()) {
			return widget;
		}
	}

	return nullptr;
}

DraggableElement *GuiFacade::draggableElement(QString const &widgetId)
{
	QList<DraggableElement *> const paletteWidgets = mMainWindow->findChildren<DraggableElement *>();
	for (DraggableElement *paletteElement : paletteWidgets) {
		if (paletteElement->id().toString() == widgetId) {
			return paletteElement;
		}
	}
	return nullptr;
}

QRect GuiFacade::property(QString const &name)
{
	QByteArray const data = name.toLocal8Bit();
	QTreeWidget const *editorTree = dynamic_cast<QTreeWidget *>(
				mMainWindow->propertyEditor()->children()[0]->children()[1]);
	for (int i = 0; i < editorTree->topLevelItemCount(); ++i) {
		QTreeWidgetItem const *item = editorTree->topLevelItem(i);
		if (item->data(0, Qt::DisplayRole).toString() == data.data()) {
			QRect const globalTarget = QRect(mMainWindow->propertyEditor()->mapToGlobal(editorTree->visualItemRect(item).center() + QPoint(10, 0))
									, editorTree->visualItemRect(item).size());
			return globalTarget;
		}
	}
}

QObject *GuiFacade::pluginGuiFacade(QString const &pluginName)
{
	return mMainWindow->toolManager().pluginGuiFacade(pluginName);
}

QWidget *GuiFacade::comboBoxProperty(QString const &name)
{
	QByteArray const data = name.toLocal8Bit();
	QTreeWidget *editorTree = dynamic_cast<QTreeWidget *>(
				mMainWindow->propertyEditor()->children()[0]->children()[1]);
	for (int i = 0; i < editorTree->topLevelItemCount(); ++i) {
		QTreeWidgetItem *item = editorTree->topLevelItem(i);
		if (item->data(0, Qt::DisplayRole).toString() == data.data()) {
			return editorTree->itemWidget(item, 1);
		}
	}
	return nullptr;
}

QWidget *GuiFacade::pluginAction(QString const &name)
{
	QList<ActionInfo > actionList = mMainWindow->toolManager().actions();
	for(ActionInfo &actionInfo : actionList) {
		if (actionInfo.action()->objectName() == name) {
			QList<QWidget *> const widgetList = actionInfo.action()->associatedWidgets();
			for (QWidget *widget : widgetList) {
				QString buttonClassName = "QToolButton";
				if (buttonClassName == widget->metaObject()->className() && widget->isVisible()) {
					return widget;
				}
			}
		}
	}
	return nullptr;
}

QWidget *GuiFacade::scene()
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
