#pragma once

#include "editor/copyPaste/pasteCommand.h"
#include "models/commands/createElementCommand.h"

namespace qReal
{
namespace commands
{

class PasteEdgeCommand : public PasteCommand
{
public:
	PasteEdgeCommand(EditorViewScene *scene
			, EdgeData const &data
			, const QPointF &offset
			, bool isGraphicalCopy
			, QHash<qReal::Id, qReal::Id> *copiedIds);
	virtual ~PasteEdgeCommand() {}

protected:
	virtual Id pasteNewInstance();
	virtual Id pasteGraphicalCopy();
	virtual void restoreElement();

private:
	EdgeData const mEdgeData;
	CreateElementCommand *mCreateCommand;
};

}
}
