/* Copyright 2014-2015 QReal Research Group, Dmitry Chernov, Dmitry Mordvinov
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

#include "paletteAPI.h"

#include "guiFacade.h"
#include "sceneAPI.h"
#include "scriptAPI.h"
#include "virtualCursor.h"
#include "palette/draggableElement.h"

using namespace qReal;
using namespace gui;

PaletteAPI::PaletteAPI(ScriptAPI &scriptAPI, MainWindow &mainWindow)
	: mScriptAPI(scriptAPI)
	, mMainWindow(mainWindow)
{
}

QString PaletteAPI::dragPaletteElement(const QString &paletteElementId, int duration
		, int xSceneCoord, int ySceneCoord)
{
	const DraggableElement *paletteElement = mScriptAPI.guiFacade().draggableElement(paletteElementId);
	const QWidget *target = paletteElement->parentWidget();

	mScriptAPI.virtualCursor().moveTo(target, duration / 2);
	mScriptAPI.virtualCursor().startDrag(paletteElement->icon());
	mScriptAPI.virtualCursor().sceneMoveTo(mMainWindow.currentTab(), duration / 2, xSceneCoord, ySceneCoord);
	mScriptAPI.virtualCursor().endDrag();
	return mScriptAPI.sceneAPI().createBlockOnScene(paletteElement, xSceneCoord, ySceneCoord);
}
