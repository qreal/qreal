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

#include "editor/copyPaste/pasteCommand.h"
#include "models/commands/createElementCommand.h"

namespace qReal {
namespace gui {
namespace editor {
namespace commands {

class PasteNodeCommand : public PasteCommand
{
public:
	PasteNodeCommand(EditorViewScene *scene
			, const NodeData &data
			, const QPointF &offset
			, bool isGraphicalCopy
			, QHash<Id, Id> *copiedIds);
	virtual ~PasteNodeCommand() {}

protected:
	virtual Id pasteNewInstance();
	virtual Id pasteGraphicalCopy();
	virtual void restoreElement();

private:
	bool explosionTargetExists() const;

	//! @returns real new pos on scene
	QPointF newPos() const;
	//! @returns pos for normal CreateElementCommand usage
	QPointF newGraphicalPos() const;
	Id newGraphicalParent() const;

	QPointF vectorFromContainer() const;

	const NodeData mNodeData;
	qReal::commands::CreateElementCommand *mCreateCommand;
};

}
}
}
}
