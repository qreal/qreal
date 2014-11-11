#include "paletteAPI.h"

#include "scriptAPI.h"

#include "palette/draggableElement.h"

using namespace qReal;
using namespace gui;

PaletteAPI::PaletteAPI(ScriptAPI *scriptAPI, MainWindow *mainWindow)
	: mScriptAPI(scriptAPI)
	, mMainWindow(mainWindow)
{
}

QString PaletteAPI::dragPaletteElement(QString const &paletteElementId, int const duration, int const xSceneCoord, int const ySceneCoord)
{
	DraggableElement *paletteElement = mScriptAPI->guiFacade()->draggableElement(paletteElementId);
	QWidget const *target = paletteElement->parentWidget();

	mScriptAPI->virtualCursor()->moveTo(target, duration / 2);

	mScriptAPI->virtualCursor()->startDrag(paletteElement->icon());

	mScriptAPI->virtualCursor()->sceneMoveTo(mMainWindow->currentTab()
			, duration / 2
			, xSceneCoord
			, ySceneCoord);

	mScriptAPI->virtualCursor()->endDrag();

	return mScriptAPI->sceneAPI()->createBlockOnScene(paletteElement, xSceneCoord, ySceneCoord);
}
