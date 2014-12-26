#pragma once

#include "controller/commands/abstractCommand.h"
#include "editor/editorViewScene.h"

namespace qReal
{
namespace commands
{

class PasteCommand : public AbstractCommand
{
public:
	virtual ~PasteCommand() {}

	Id result() const;

protected:
	PasteCommand(EditorViewScene *scene
			, QPointF const &offset
			, bool isGraphicalCopy
			, QHash<Id, Id> *copiedIds);

	virtual bool execute();
	virtual bool restoreState();

	virtual Id pasteNewInstance() = 0;
	virtual Id pasteGraphicalCopy() = 0;
	virtual void restoreElement() = 0;

	EditorViewScene *mScene;
	QPointF const mOffset;
	QHash<Id, Id> *mCopiedIds;
	Id mResult;

private:
	bool const mIsGraphicalCopy;
};

}
}
