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

#include "sceneAPI.h"

#include <editor/editorView.h>
#include <editor/editorViewScene.h>

#include "mainWindow.h"
#include "palette/draggableElement.h"
#include "scriptAPI.h"
#include "virtualCursor.h"

using namespace qReal;
using namespace gui;
using namespace qReal::gui::editor;

SceneAPI::SceneAPI(ScriptAPI &scriptAPI, MainWindow &mainWindow)
	: mScriptAPI(scriptAPI)
	, mMainWindow(mainWindow)
{
}

void SceneAPI::drawLink(const QString &fromElementId, const QString &toElementId, int duration)
{
	if (!mMainWindow.getCurrentTab()) {
		return;
	}

	const EditorViewScene * const scene = &mMainWindow.getCurrentTab()->editorViewScene();
	const EditorView * const sceneView = mMainWindow.getCurrentTab();

	const NodeElement * const toNode = scene->getNodeById(Id::loadFromString(toElementId));
	const NodeElement * const fromNode = scene->getNodeById(Id::loadFromString(fromElementId));

	if (toNode && fromNode) {
		mScriptAPI.virtualCursor().sceneMoveTo(sceneView->viewport()
				, duration / 2
				, sceneView->mapFromScene(fromNode->pos()).x()
				, sceneView->mapFromScene(fromNode->pos()).y());

		mScriptAPI.virtualCursor().rightButtonPress(sceneView->viewport());

		mScriptAPI.virtualCursor().sceneMoveTo(sceneView->viewport()
				, duration / 2
				, sceneView->mapFromScene(toNode->pos()).x()
				, sceneView->mapFromScene(toNode->pos()).y());

		mScriptAPI.virtualCursor().rightButtonRelease(sceneView->viewport(), 50);
	}
}

QString SceneAPI::createBlockOnScene(const DraggableElement * const paletteElement
		, int xSceneCoord, int ySceneCoord)
{
	if (EditorView * const currentTab = mMainWindow.getCurrentTab()) {
		const Id elementId = paletteElement->id().sameTypeId();
		const QMimeData * const mimeData = paletteElement->mimeData(elementId);
		currentTab->mutableScene().createElement(
				paletteElement->mimeData(elementId)
				, currentTab->mapToScene(QPoint(xSceneCoord, ySceneCoord)));

		QByteArray itemData = mimeData->data("application/x-real-uml-data");
		QDataStream inStream(&itemData, QIODevice::ReadOnly);
		QString uuid;
		inStream >> uuid;
		return uuid;
	}

	return QString();
}

QStringList SceneAPI::nodeList(const QString &diagram, const QString &element)
{
	if (!mMainWindow.getCurrentTab()) {
		return {};
	}

	/// @todo: Rewrite it using models
	const QList<QGraphicsItem *> items = mMainWindow.getCurrentTab()->editorViewScene().items();
	QStringList result;
	for (const QGraphicsItem * const item : items) {
		const NodeElement * const node = dynamic_cast<NodeElement const *>(item);
		if (node && node->id().diagram() == diagram && node->id().element() == element) {
			result << node->id().toString();
		}
	}

	return result;
}
