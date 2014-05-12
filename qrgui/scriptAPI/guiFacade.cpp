#include "guiFacade.h"

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
		for(QAction *action : actionList) {
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
	} else if (!type.compare("2dModel")) {
		QList<ToolPluginInterface *> pluginList = mMainWindow->toolManager().getPlugins();
		for (ToolPluginInterface *plugin : pluginList) {
			QList<ActionInfo> actionList = plugin->actions();
			for (ActionInfo &actionInfo : actionList) {
				if (!actionInfo.name().compare(name)){
					QAction *action = actionInfo.action();
					QList<QWidget *> widgetList =action->associatedWidgets();
					for(QWidget *widget : widgetList) {
						QString buttonClassName = "QToolButton";
						if (!buttonClassName.compare(widget->metaObject()->className())) {
							return widget;
						}
					}
				}
			}
		}
	}
}

DraggableElement* GuiFacade::draggableElement(QString const &widgetId) const
{
	QList<DraggableElement *> paletteWidgets = mMainWindow->findChildren<DraggableElement *>();
	for(DraggableElement *paletteElement : paletteWidgets){
		if(paletteElement->id() == Id::loadFromString(widgetId)) {
			return paletteElement;
		}
	}
}

QWidget* GuiFacade::widget2dModel(QString const &type, QString const &name)
{
}

ToolPluginInterface* GuiFacade::plugin()
{
	return mRobotsPlugin;
}
