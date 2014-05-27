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
	if (!type.compare("PushButton")) {
		QList<QPushButton *> buttonList = mD2ModelWidget->findChildren<QPushButton *>();
		for(QPushButton *button : buttonList) {
			if (!name.compare(button->objectName())) {
				return button;
			}
		}
	} else {
		return nullptr;
	}
}

QWidget *RobotsGuiFacade::d2ModelScene()
{
	return mD2ModelWidget->scene()->views()[0]->viewport();
}

QWidget* RobotsGuiFacade::d2ModelWidget()
{
	return mD2ModelWidget;
}
