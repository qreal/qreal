#include <QtWidgets/QPushButton>
#include <QtWidgets/QComboBox>

#include "commonTwoDModel/engine/twoDModelGuiFacade.h"
#include "view/d2ModelWidget.h"

using namespace twoDModel::engine;
using namespace twoDModel::view;

TwoDModelGuiFacade::TwoDModelGuiFacade(D2ModelWidget &view)
	: mD2ModelWidget(view)
{
}

QWidget *TwoDModelGuiFacade::widget(QString const &type, QString const &name)
{
	QList<QWidget *> const widgetList = mD2ModelWidget.findChildren<QWidget *>(name);

	for (QWidget *widget : widgetList) {
		if (type == widget->metaObject()->className() && widget->isVisible()) {
			return widget;
		}
	}

	return nullptr;
}

QWidget *TwoDModelGuiFacade::d2ModelScene()
{
	return mD2ModelWidget.scene()->views()[0]->viewport();
}

QWidget *TwoDModelGuiFacade::d2ModelWidget()
{
	return &mD2ModelWidget;
}
