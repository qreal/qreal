#include <QtWidgets/QPushButton>
#include <QtWidgets/QComboBox>

#include <qrutils/widgetFinder.h>
#include <qrutils/qRealDialog.h>

#include "twoDModel/engine/twoDModelGuiFacade.h"
#include "twoDModel/engine/view/twoDModelWidget.h"
#include "src/engine/view/scene/twoDModelScene.h"

using namespace twoDModel::engine;

TwoDModelGuiFacade::TwoDModelGuiFacade(view::TwoDModelWidget &view)
	: mD2ModelWidget(view)
{
}

QWidget *TwoDModelGuiFacade::widget(QString const &type, QString const &name) const
{
	return utils::WidgetFinder::widget(&mD2ModelWidget, type, name);
}

QWidget *TwoDModelGuiFacade::twoDModelSceneViewport() const
{
	return mD2ModelWidget.scene()->views()[0]->viewport();
}

QWidget *TwoDModelGuiFacade::twoDModelWidget() const
{
	return &mD2ModelWidget;
}

QWidget *TwoDModelGuiFacade::separateTwoDModelWindow() const
{
	if (dynamic_cast<utils::QRealDialog *>(mD2ModelWidget.topLevelWidget())) {
		return &mD2ModelWidget;
	}

	return nullptr;
}
