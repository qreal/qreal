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
