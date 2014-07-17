#include "sceneAPI.h"

#include "scriptAPI.h"

#include "mainwindow/mainWindow.h"
#include "mainwindow/palette/draggableElement.h"

using namespace qReal;
using namespace gui;

SceneAPI::SceneAPI(ScriptAPI *scriptAPI, MainWindow *mainWindow)
	: mScriptAPI(scriptAPI)
	, mMainWindow(mainWindow)
{
}

QString SceneAPI::initialNode()
{
	QList<QGraphicsItem *> items = mMainWindow->getCurrentTab()->editorViewScene()->items();
	foreach (QGraphicsItem *item, items) {
		NodeElement *node = dynamic_cast<NodeElement*>(item);
		if (node) {
			if (!node->id().diagram().compare("RobotsDiagram") && !node->id().element().compare("InitialNode")) {
				return node->id().toString();
			}
		}
	}
	return "";
}

void SceneAPI::drawLink(QString const &fromElementId, QString const &toElementId, int const duration)
{
	NodeElement *fromNode = mMainWindow->getCurrentTab()->editorViewScene()->getNodeById(Id::loadFromString(fromElementId));
	mScriptAPI->virtualCursor()->sceneMoveTo(mMainWindow->getCurrentTab()->editorViewScene()->views()[0]->viewport()
			, duration/2
			, fromNode->pos().x() + 100
			, fromNode->pos().y() + 10);

	mScriptAPI->virtualCursor()->rightButtonPress(mMainWindow->getCurrentTab()->viewport());

	NodeElement *toNode = mMainWindow->getCurrentTab()->editorViewScene()->getNodeById(Id::loadFromString(toElementId));
	mScriptAPI->virtualCursor()->sceneMoveTo(mMainWindow->getCurrentTab()->editorViewScene()->views()[0]->viewport()
			, duration/2
			, toNode->pos().x() + 100
			, toNode->pos().y() + 10);

	mScriptAPI->virtualCursor()->rightButtonRelease(mMainWindow->getCurrentTab()->viewport());
}

QString SceneAPI::createBlockOnScene(DraggableElement const *paletteElement, int const xSceneCoord, int const ySceneCoord)
{
	Id elementId(paletteElement->id(), QUuid::createUuid().toString());
	QMimeData *mimeData = paletteElement->mimeData(elementId);
	mMainWindow->getCurrentTab()->editorViewScene()->createElement(
				paletteElement->mimeData(elementId)
				, QPoint(xSceneCoord - 100, ySceneCoord + 20)
				, false
				, nullptr
				, true);

	QByteArray itemData = mimeData->data("application/x-real-uml-data");
	QDataStream inStream(&itemData, QIODevice::ReadOnly);
	QString uuid = "";
	inStream >> uuid;
	Id const sceneId = Id::loadFromString(uuid);
	return sceneId.toString();
}
