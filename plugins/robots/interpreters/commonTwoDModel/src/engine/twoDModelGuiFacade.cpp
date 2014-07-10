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

QWidget* TwoDModelGuiFacade::widget(QString const &type, QString const &name)
{
	if (type == "PushButton") {
		QPushButton *button = mD2ModelWidget.findChild<QPushButton *>(name);
		return button;
	} else {
		return nullptr;
	}
}

QWidget *TwoDModelGuiFacade::d2ModelScene()
{
	return mD2ModelWidget.scene()->views()[0]->viewport();
}

QWidget *TwoDModelGuiFacade::d2ModelWidget()
{
	return &mD2ModelWidget;
}
