#include "guiFacade.h"

#include "mainwindow/mainWindow.h"
#include "mainwindow/palette/draggableElement.h"
#include "toolPluginInterface/toolPluginInterface.h"

using namespace qReal;
using namespace gui;

GuiFacade::GuiFacade(MainWindow *mainWindow)
	: mMainWindow(mainWindow)
	, mRobotsPlugin(nullptr)
{
	QList<ToolPluginInterface *> pluginList = mMainWindow->toolManager().getPlugins();
	for (ToolPluginInterface *plugin : pluginList) {
		if (!plugin->pluginName().compare("robots")) {
			mRobotsPlugin = plugin;
		}
	}
}

QWidget* GuiFacade::widget(QString const &type, QString const &name)
{
	if (!type.compare("Action")) {
		QList<QAction *> actionList = mMainWindow->findChildren<QAction *>();
		for(QAction const *action : actionList) {
			if (!action->objectName().compare(name)) {
				QList<QWidget *> widgetList =action->associatedWidgets();
				for(QWidget *widget : widgetList) {
					QString buttonClassName = "QToolButton";
					if (!buttonClassName.compare(widget->metaObject()->className())) {
						return widget;
					}
				}
			}
		}
		QWidget *action = robotAction(name);
		if (action) {
			return action;
		}
	} else if (!type.compare("ComboBox")) {
		QList<QComboBox *> comboBoxList = mMainWindow->findChildren<QComboBox*>();
		for(QComboBox *comboBox : comboBoxList) {
			if (!comboBox->objectName().compare(name) && comboBox->isVisible()) {
				return comboBox;
			}
		}
		QWidget *comboBox = comboBoxProperty(name);
		if (comboBox) {
			return comboBox;
		}
	} else if (!type.compare("Scene")) {
		return mMainWindow->getCurrentTab()->viewport();
	} else if (!type.compare("PropertyEditor")) {
		PropertyEditorView const *propertyEditor = mMainWindow->findChild<PropertyEditorView *>("propertyEditor");
		return propertyEditor->
				findChild<QtTreePropertyBrowser *>()->
				findChild<QTreeWidget *>()->viewport();
	}
	return nullptr;
}

DraggableElement* GuiFacade::draggableElement(QString const &widgetId)
{
	QList<DraggableElement *> paletteWidgets = mMainWindow->findChildren<DraggableElement *>();
	for(DraggableElement *paletteElement : paletteWidgets){
		if(!paletteElement->id().toString().compare(widgetId)) {
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

ToolPluginInterface* GuiFacade::plugin()
{
	return mRobotsPlugin;
}

QWidget* GuiFacade::comboBoxProperty(QString const &name)
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

QWidget* GuiFacade::robotAction(QString const &name)
{
	QList<ActionInfo > actionList = mRobotsPlugin->actions();
	for(ActionInfo &actionInfo : actionList) {
		if (!actionInfo.action()->objectName().compare(name)) {
			QList<QWidget *> widgetList = actionInfo.action()->associatedWidgets();
			for(QWidget *widget : widgetList) {
				QString buttonClassName = "QToolButton";
				if (!buttonClassName.compare(widget->metaObject()->className()) && widget->isVisible()) {
					return widget;
				}
			}
		}
	}
	return nullptr;
}
