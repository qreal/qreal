/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

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

QWidget *TwoDModelGuiFacade::widget(const QString &type, const QString &name) const
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
