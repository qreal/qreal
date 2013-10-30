#pragma once

#include "view/copyPaste/pasteCommand.h"
#include "controller/commands/createElementCommand.h"

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
	//! @returns real new pos on scene
	QPointF newPos() const;
	//! @returns pos for normal CreateElementCommand usage
	QPointF newGraphicalPos() const;
	Id newGraphicalParent() const;

	QPointF vectorFromContainer() const;

	NodeData const mNodeData;
	CreateElementCommand *mCreateCommand;
};

}
}
