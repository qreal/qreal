#pragma once

#include "pasteCommand.h"
#include "../../controller/commands/createElementCommand.h"

namespace qReal
{
namespace commands
{

class PasteNodeCommand : public PasteCommand
{
public:
	PasteNodeCommand(EditorViewScene *scene
			, EditorViewMViface const *mvIface
			, NodeData const &data
			, QPointF const &offset
			, bool isGraphicalCopy
			, QHash<Id, Id> *copiedIds);
	virtual ~PasteNodeCommand() {}

protected:
	virtual Id pasteNewInstance();
	virtual Id pasteGraphicalCopy();
	virtual void restoreElement();

private:
	QPointF getNewPos() const;

	NodeData const mNodeData;
	CreateElementCommand *mCreateCommand;
};

}
}
