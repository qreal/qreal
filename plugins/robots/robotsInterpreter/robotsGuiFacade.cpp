#include "robotsGuiFacade.h"
#include "robotsPlugin.h"

using namespace qReal;
using namespace interpreters::robots;
using namespace interpreters::robots::details;

RobotsGuiFacade::RobotsGuiFacade(d2Model::D2ModelWidget *d2ModelWidget)
	: mD2ModelWidget(d2ModelWidget)
{
}

QWidget* RobotsGuiFacade::widget(QString const &type, QString const &name)
{
//	if (!type.compare("Action")) {
//		QList<QActionInfo *> actionList = mMainWindow->findChildren<QAction *>();
//		for(QAction *action : actionList) {
//			if (!action->objectName().compare(name)) {
//				QList<QWidget *> widgetList =action->associatedWidgets();
//				for(QWidget *widget : widgetList) {
//					QString buttonClassName = "QToolButton";
//					if (!buttonClassName.compare(widget->metaObject()->className())) {
//						return widget;
//					}
//				}
//			}
//		}
//		return nullptr;
//	}
}

QWidget* RobotsGuiFacade::d2ModelWidget()
{
	return mD2ModelWidget;
}
