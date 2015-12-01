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

#pragma once

#include <QtCore/QObject>

#include "editor/nodeElement.h"
#include "editor/edgeElement.h"
#include "editor/serializationData.h"

namespace qReal {
namespace gui {
namespace editor {

class EditorViewScene;

class ClipboardHandler
{
public:
	ClipboardHandler(EditorViewScene &scene, Controller &controller);

	void cut();
	void copy();
	void paste(bool isGraphicalCopy);

private:
	void pushDataToClipboard(QList<NodeData> const &nodesData, QList<EdgeData> const &edgesData);
	QList<NodeElement *> getNodesForCopying();
	QList<NodeData> getNodesData(QList<NodeElement *> const &nodes);
	QList<EdgeData> getEdgesData();
	void addChildren(NodeElement *node, QList<NodeElement *> &nodes);

	EditorViewScene &mScene;
	Controller &mController;
};

}
}
}
