#include "sceneAPI.h"

#include <editor/editorView.h>
#include <editor/editorViewScene.h>

#include "mainwindow/mainWindow.h"
#include "mainwindow/palette/draggableElement.h"
#include "scriptAPI.h"

using namespace qReal;
using namespace gui;

SceneAPI::SceneAPI(ScriptAPI *scriptAPI, MainWindow *mainWindow)
	: mScriptAPI(scriptAPI)
	, mMainWindow(mainWindow)
{
}

void SceneAPI::drawLink(QString const &fromElementId, QString const &toElementId, int const duration)
{
	EditorViewScene const *scene = &mMainWindow->getCurrentTab()->editorViewScene();
	EditorView *sceneView = mMainWindow->getCurrentTab();

	NodeElement *toNode = scene->getNodeById(Id::loadFromString(toElementId));
	NodeElement *fromNode = scene->getNodeById(Id::loadFromString(fromElementId));

	if (toNode && fromNode) {
		mScriptAPI->virtualCursor()->sceneMoveTo(sceneView->viewport()
				, duration / 2
				, sceneView->mapFromScene(fromNode->pos()).x()
				, sceneView->mapFromScene(fromNode->pos()).y());

		mScriptAPI->virtualCursor()->rightButtonPress(sceneView->viewport());

		mScriptAPI->virtualCursor()->sceneMoveTo(sceneView->viewport()
				, duration / 2
				, sceneView->mapFromScene(toNode->pos()).x()
				, sceneView->mapFromScene(toNode->pos()).y());

		mScriptAPI->virtualCursor()->rightButtonRelease(sceneView->viewport(), 50);
	}
}

QString SceneAPI::createBlockOnScene(DraggableElement const *paletteElement, int const xSceneCoord, int const ySceneCoord)
{
	EditorView *currentTab = mMainWindow->getCurrentTab();
	if (currentTab) {
		Id elementId(paletteElement->id(), QUuid::createUuid().toString());
		QMimeData *mimeData = paletteElement->mimeData(elementId);
		currentTab->mutableScene().createElement(
				paletteElement->mimeData(elementId)
				, currentTab->mapToScene(QPoint(xSceneCoord, ySceneCoord))
				, false
				, nullptr
				, true);

		QByteArray itemData = mimeData->data("application/x-real-uml-data");
		QDataStream inStream(&itemData, QIODevice::ReadOnly);
		QString uuid;
		inStream >> uuid;
		Id const sceneId = Id::loadFromString(uuid);
		return sceneId.toString();
	}
	return "";
}

QStringList SceneAPI::nodeList(QString const &diagram, QString const &element)
{
	QList<QGraphicsItem *> items = mMainWindow->getCurrentTab()->editorViewScene().items();
	QStringList result;
	foreach (QGraphicsItem *item, items) {
		NodeElement *node = dynamic_cast<NodeElement*>(item);
		if (node) {
			if (!node->id().diagram().compare(diagram) && !node->id().element().compare(element)) {
				result.append(node->id().toString());
			}
		}
	}

	return result;
}
